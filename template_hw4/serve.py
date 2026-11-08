from http.server import HTTPServer, SimpleHTTPRequestHandler

# Handler for cross origin isolation so SharedArrayBuffers are allowed
class CORSRequestHandler(SimpleHTTPRequestHandler):
    def end_headers(self):
        self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
        self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')
        super().end_headers()

# Set up HTTP server
server_address = ('localhost', 8000)
httpd = HTTPServer(server_address, CORSRequestHandler)

print("Server running at http://localhost:8000/")
httpd.serve_forever()