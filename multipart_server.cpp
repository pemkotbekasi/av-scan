#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

namespace beast = boost::beast;           
namespace http = beast::http;             
namespace net = boost::asio;              
using tcp = boost::asio::ip::tcp;       

// Fungsi untuk memindai file menggunakan ClamAV
std::string scan_with_clamav(const std::string& filepath) {
    std::string command = "clamscan " + filepath;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return "Error: Could not run ClamAV.";
    
    char buffer[128];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    return result;
}

// Fungsi untuk menangani permintaan HTTP
void handle_request(http::request<http::string_body>& req, http::response<http::string_body>& res) {
    const size_t MAX_UPLOAD_SIZE = 10 * 1024 * 1024; // 10 MB

    if (req.method() == http::verb::post) {
        // Dapatkan isi dari multipart/form-data
        auto it = req.find("Content-Type");
        if (it != req.end()) {
            std::string content_type = it->value().to_string();
            if (content_type.find("multipart/form-data") != std::string::npos) {
                // Periksa ukuran unggahan
                if (req.body().size() > MAX_UPLOAD_SIZE) {
                    res.result(http::status::payload_too_large);
                    res.set(http::field::content_type, "text/plain");
                    res.body() = "File size exceeds the maximum limit.";
                    res.prepare_payload();
                    return;
                }

                // Simpan file ke /tmp
                std::string filepath = "/tmp/uploaded_file"; // Simpan di /tmp
                std::ofstream ofs(filepath, std::ios::binary);
                ofs << req.body();
                ofs.close();

                // Pemindaian file
                std::string clamav_result = scan_with_clamav(filepath);

                // Siapkan respons
                res.result(http::status::ok);
                res.set(http::field::content_type, "text/plain");
                res.body() = clamav_result;
                res.prepare_payload();

                // Hapus file setelah pemindaian
                boost::filesystem::remove(filepath);
                return;
            }
        }
    }
    res.result(http::status::bad_request);
    res.set(http::field::content_type, "text/plain");
    res.body() = "Invalid request.";
    res.prepare_payload();
}

// Fungsi utama
int main() {
    try {
        net::io_context io_context;

        // Membuat endpoint untuk menerima koneksi
        tcp::acceptor acceptor(io_context, {tcp::v4(), 8080});
        for (;;) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            // Menerima permintaan
            beast::flat_buffer buffer;
            http::request<http::string_body> req;
            http::read(socket, buffer, req);

            // Menangani permintaan
            http::response<http::string_body> res;
            handle_request(req, res);

            // Mengirim respons
            http::write(socket, res);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
