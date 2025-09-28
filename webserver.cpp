#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <memory>
#include <thread>
#include <chrono>
#include <map>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

class SimpleHTTPServer {
private:
    int server_fd;
    int port;
    
public:
    SimpleHTTPServer(int port = 5000) : port(port), server_fd(-1) {}
    
    ~SimpleHTTPServer() {
        if (server_fd != -1) {
            close(server_fd);
        }
    }
    
    bool start() {
        // Create socket
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == 0) {
            std::cerr << "Failed to create socket" << std::endl;
            return false;
        }
        
        // Set socket options
        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
            std::cerr << "Failed to set socket options" << std::endl;
            return false;
        }
        
        // Bind socket
        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);
        
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
            std::cerr << "Failed to bind socket" << std::endl;
            return false;
        }
        
        // Listen for connections
        if (listen(server_fd, 3) < 0) {
            std::cerr << "Failed to listen" << std::endl;
            return false;
        }
        
        std::cout << "Server started on port " << port << std::endl;
        return true;
    }
    
    void run() {
        while (true) {
            int new_socket = accept(server_fd, nullptr, nullptr);
            if (new_socket < 0) {
                continue;
            }
            
            // Handle request in a separate thread
            std::thread([this, new_socket]() {
                handleRequest(new_socket);
                close(new_socket);
            }).detach();
        }
    }
    
private:
    void handleRequest(int socket) {
        char buffer[4096] = {0};
        read(socket, buffer, 4096);
        
        std::string request(buffer);
        std::istringstream request_stream(request);
        std::string method, path, version;
        request_stream >> method >> path >> version;
        
        // Parse headers
        std::map<std::string, std::string> headers;
        std::string line;
        while (std::getline(request_stream, line) && line != "\r") {
            size_t colon_pos = line.find(':');
            if (colon_pos != std::string::npos) {
                std::string key = line.substr(0, colon_pos);
                std::string value = line.substr(colon_pos + 1);
                // Remove leading/trailing whitespace
                key.erase(0, key.find_first_not_of(" \t\r\n"));
                key.erase(key.find_last_not_of(" \t\r\n") + 1);
                value.erase(0, value.find_first_not_of(" \t\r\n"));
                value.erase(value.find_last_not_of(" \t\r\n") + 1);
                headers[key] = value;
            }
        }
        
        // Read body if present
        std::string body;
        if (headers.find("Content-Length") != headers.end()) {
            int content_length = std::stoi(headers["Content-Length"]);
            body.resize(content_length);
            ssize_t bytes_read = read(socket, &body[0], content_length);
            if (bytes_read != content_length) {
                std::cout << "DEBUG: Expected " << content_length << " bytes, got " << bytes_read << std::endl;
            }
        }
        
        // Route request
        std::string response;
        if (path == "/health") {
            response = handleHealth();
        } else if (path == "/scan" && method == "POST") {
            response = handleScan(body);
        } else if (path == "/parse" && method == "POST") {
            response = handleParse(body);
        } else if (path == "/codegen" && method == "POST") {
            response = handleCodegen(body);
        } else if (path == "/compile" && method == "POST") {
            response = handleCompile(body);
        } else {
            response = handle404();
        }
        
        // Send response
        send(socket, response.c_str(), response.length(), 0);
    }
    
    
    std::string handleHealth() {
        std::string json = R"({"status":"healthy","message":"WLP4 Compiler API Server is running","endpoints":["/health","/scan","/parse","/codegen","/compile"]})";
        return createHTTPResponse(json, "application/json");
    }
    
    std::string handleScan(const std::string& body) {
        std::cout << "DEBUG: Received body: '" << body << "'" << std::endl;
        std::string input_code = parseFormData(body, "code");
        std::cout << "DEBUG: Parsed code: '" << input_code << "'" << std::endl;
        if (input_code.empty()) {
            return createHTTPResponse(R"({"success":false,"error":"No input code provided"})", "application/json");
        }
        
        auto result = runCompilerStage("./01_scan/wlp4scan", input_code);
        
        std::ostringstream json;
        json << "{\"success\":" << (result.success ? "true" : "false");
        if (result.success) {
            json << ",\"output\":\"" << escapeJson(result.output) << "\"";
        } else {
            json << ",\"error\":\"" << escapeJson(result.error) << "\"";
        }
        json << "}";
        
        return createHTTPResponse(json.str(), "application/json");
    }
    
    std::string handleParse(const std::string& body) {
        std::string input_tokens = parseFormData(body, "tokens");
        if (input_tokens.empty()) {
            return createHTTPResponse(R"({"success":false,"error":"No token stream provided"})", "application/json");
        }
        
        auto result = runCompilerStage("./02_parse/wlp4parse", input_tokens);
        
        std::ostringstream json;
        json << "{\"success\":" << (result.success ? "true" : "false");
        if (result.success) {
            json << ",\"output\":\"" << escapeJson(result.output) << "\"";
        } else {
            json << ",\"error\":\"" << escapeJson(result.error) << "\"";
        }
        json << "}";
        
        return createHTTPResponse(json.str(), "application/json");
    }
    
    std::string handleCodegen(const std::string& body) {
        std::string input_parse_tree = parseFormData(body, "parse_tree");
        if (input_parse_tree.empty()) {
            return createHTTPResponse(R"({"success":false,"error":"No parse tree provided"})", "application/json");
        }
        
        auto result = runCompilerStage("./03_codegen/wlp4gen", input_parse_tree);
        
        std::ostringstream json;
        json << "{\"success\":" << (result.success ? "true" : "false");
        if (result.success) {
            json << ",\"output\":\"" << escapeJson(result.output) << "\"";
        } else {
            json << ",\"error\":\"" << escapeJson(result.error) << "\"";
        }
        json << "}";
        
        return createHTTPResponse(json.str(), "application/json");
    }
    
    
    std::string handleCompile(const std::string& body) {
        std::string input_code = parseFormData(body, "code");
        if (input_code.empty()) {
            return createHTTPResponse(R"({"success":false,"error":"No input code provided"})", "application/json");
        }
        
        // Run complete pipeline: scan -> parse -> codegen
        auto scan_result = runCompilerStage("./01_scan/wlp4scan", input_code);
        if (!scan_result.success) {
            std::ostringstream json;
            json << "{\"success\":false,\"error\":\"Scanner error: " << escapeJson(scan_result.error) << "\"}";
            return createHTTPResponse(json.str(), "application/json");
        }
        
        auto parse_result = runCompilerStage("./02_parse/wlp4parse", scan_result.output);
        if (!parse_result.success) {
            std::ostringstream json;
            json << "{\"success\":false,\"error\":\"Parser error: " << escapeJson(parse_result.error) << "\"}";
            return createHTTPResponse(json.str(), "application/json");
        }
        
        auto codegen_result = runCompilerStage("./03_codegen/wlp4gen", parse_result.output);
        if (!codegen_result.success) {
            std::ostringstream json;
            json << "{\"success\":false,\"error\":\"Code generator error: " << escapeJson(codegen_result.error) << "\"}";
            return createHTTPResponse(json.str(), "application/json");
        }
        
        std::ostringstream json;
        json << "{\"success\":true";
        json << ",\"output\":\"" << escapeJson(codegen_result.output) << "\"";
        json << ",\"tokens\":\"" << escapeJson(scan_result.output) << "\"";
        json << ",\"parse_tree\":\"" << escapeJson(parse_result.output) << "\"";
        json << "}";
        
        return createHTTPResponse(json.str(), "application/json");
    }
    
    std::string handle404() {
        std::string json = R"({"error":"404 Not Found","message":"The requested endpoint was not found","available_endpoints":["/health","/scan","/parse","/codegen","/compile"]})";
        return createHTTPResponse(json, "application/json", 404);
    }
    
    struct CompilerResult {
        bool success;
        std::string output;
        std::string error;
    };
    
    CompilerResult runCompilerStage(const std::string& executable_path, const std::string& input_data) {
        CompilerResult result;
        
        // Check if executable exists
        std::ifstream file(executable_path);
        if (!file.good()) {
            result.success = false;
            result.error = "Executable not found: " + executable_path;
            return result;
        }
        file.close();
        
        // Create temporary files for input and output
        std::string temp_input = "/tmp/wlp4_input_" + std::to_string(std::chrono::high_resolution_clock::now().time_since_epoch().count());
        std::string temp_output = "/tmp/wlp4_output_" + std::to_string(std::chrono::high_resolution_clock::now().time_since_epoch().count());
        
        try {
            // Write input to temporary file
            std::ofstream input_file(temp_input);
            input_file << input_data;
            input_file.close();
            
            // Run the executable
            std::string command = executable_path + " < " + temp_input + " > " + temp_output + " 2>&1";
            int exit_code = std::system(command.c_str());
            
            // Read output
            std::ifstream output_file(temp_output);
            std::stringstream buffer;
            buffer << output_file.rdbuf();
            output_file.close();
            
            result.output = buffer.str();
            
            if (exit_code == 0) {
                result.success = true;
                result.error = "";
            } else {
                result.success = false;
                result.error = result.output.empty() ? "Unknown error" : result.output;
            }
            
        } catch (const std::exception& e) {
            result.success = false;
            result.error = "Exception: " + std::string(e.what());
        }
        
        // Clean up temporary files
        std::remove(temp_input.c_str());
        std::remove(temp_output.c_str());
        
        return result;
    }
    
    std::string parseFormData(const std::string& body, const std::string& field_name) {
        // Simple form data parsing
        std::string search = field_name + "=";
        size_t start = body.find(search);
        if (start == std::string::npos) {
            return "";
        }
        start += search.length();
        
        size_t end = body.find("&", start);
        if (end == std::string::npos) {
            end = body.length();
        }
        
        std::string value = body.substr(start, end - start);
        
        // URL decode
        std::string decoded;
        for (size_t i = 0; i < value.length(); ++i) {
            if (value[i] == '%' && i + 2 < value.length()) {
                std::string hex = value.substr(i + 1, 2);
                char c = static_cast<char>(std::strtol(hex.c_str(), nullptr, 16));
                decoded += c;
                i += 2;
            } else if (value[i] == '+') {
                decoded += ' ';
            } else {
                decoded += value[i];
            }
        }
        
        return decoded;
    }
    
    std::string escapeJson(const std::string& str) {
        std::string escaped;
        for (char c : str) {
            switch (c) {
                case '"': escaped += "\\\""; break;
                case '\\': escaped += "\\\\"; break;
                case '\n': escaped += "\\n"; break;
                case '\r': escaped += "\\r"; break;
                case '\t': escaped += "\\t"; break;
                default: escaped += c; break;
            }
        }
        return escaped;
    }
    
    std::string createHTTPResponse(const std::string& content, const std::string& content_type, int status_code = 200) {
        std::ostringstream response;
        response << "HTTP/1.1 " << status_code << " OK\r\n";
        response << "Content-Type: " << content_type << "\r\n";
        response << "Content-Length: " << content.length() << "\r\n";
        response << "Access-Control-Allow-Origin: *\r\n";
        response << "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n";
        response << "Access-Control-Allow-Headers: Content-Type\r\n";
        response << "\r\n";
        response << content;
        return response.str();
    }
};

int main(int argc, char* argv[]) {
    int port = 5000;
    
    if (argc > 1) {
        try {
            port = std::stoi(argv[1]);
        } catch (const std::exception& e) {
            std::cerr << "Invalid port number: " << argv[1] << std::endl;
            std::cerr << "Usage: " << argv[0] << " [port]" << std::endl;
            return 1;
        }
    }
    
    std::cout << "Starting WLP4 Compiler API Server..." << std::endl;
    std::cout << "Available API endpoints:" << std::endl;
    std::cout << "  GET  /health - Health check" << std::endl;
    std::cout << "  POST /scan - Scanner worker" << std::endl;
    std::cout << "  POST /parse - Parser worker" << std::endl;
    std::cout << "  POST /codegen - Code generator worker" << std::endl;
    std::cout << "  POST /compile - Complete compilation pipeline" << std::endl;
    std::cout << std::endl;
    std::cout << "API server will start on http://localhost:" << port << std::endl;
    
    // Check if worker executables exist
    std::vector<std::pair<std::string, std::string>> workers = {
        {"Scanner Worker", "./01_scan/wlp4scan"},
        {"Parser Worker", "./02_parse/wlp4parse"},
        {"Code Generator Worker", "./03_codegen/wlp4gen"}
    };
    
    std::vector<std::string> missing_workers;
    for (const auto& worker : workers) {
        std::ifstream file(worker.second);
        if (!file.good()) {
            missing_workers.push_back(worker.first + ": " + worker.second);
        }
        file.close();
    }
    
    if (!missing_workers.empty()) {
        std::cout << "ERROR: The following compiler workers are missing:" << std::endl;
        for (const auto& worker : missing_workers) {
            std::cout << "  - " << worker << std::endl;
        }
        std::cout << std::endl;
        std::cout << "Please build the webserver with workers first:" << std::endl;
        std::cout << "  make" << std::endl;
        std::cout << std::endl;
        std::cout << "This will automatically build all compiler worker stages." << std::endl;
        return 1;
    } else {
        std::cout << "All compiler workers are available:" << std::endl;
        for (const auto& worker : workers) {
            std::cout << "  ✓ " << worker.first << std::endl;
        }
        std::cout << std::endl;
    }
    
    SimpleHTTPServer server(port);
    if (!server.start()) {
        std::cerr << "Failed to start server" << std::endl;
        return 1;
    }
    
    server.run();
    
    return 0;
}