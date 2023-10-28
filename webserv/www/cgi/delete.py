import os, sys, cgi, datetime

def print_links():
    print("<p> Go to:")
    print("<a href=\"index.html\">Index</a>")
    print("<a href=\"get.py?\">Get</a>")
    print("<a href=\"post.html\">Post</a>")
    print("</p>")

method = os.environ['REQUEST_METHOD']

if method == 'GET':
    print("<h1>Delete Page</h1>")

if 'UPLOAD_PATH' in os.environ:
    folder = os.environ['UPLOAD_PATH']
else:
    root_folder = os.environ['ROOT_FOLDER']
    folder = os.path.join(root_folder, 'upload')

if not os.path.exists(folder):
    print(f"<p>Upload folder is not set or not match.</p>")
    print_links()
    sys.exit(0)

files = os.listdir(folder)

if files:
    print("<h2>Uploaded Files</h2>")
    print("<ul>")
    for file in files:
        print(f"<p>{file} <a href=\"#\" data-file=\"{file}\">Remove</a></p>")
    
    print_links()
    print("</ul>")
else:
    print("<p>Upload folder is empty.</p>")
    print_links()

print("""
<script>
    const removeLinks = document.querySelectorAll("a[data-file]");

    removeLinks.forEach(link => {
        link.addEventListener("click", function (event) {
            event.preventDefault();

            const file = this.getAttribute("data-file");
            if (confirm("Are you sure you want to delete " + file + "?")) {
                deleteFile(file);
            }
        });
    });

    function deleteFile(file) {
        fetch(`/upload/${file}`, { method: "DELETE" })
            .then(response => {
                if (response.status === 200) {
                    alert(`File ${file} has been deleted.`);
                    location.reload();
                } else {
                    alert(`Failed to delete file ${file}.`);
                }
            })
            .catch(error => {
                console.error("Error deleting file: " + error);
            });
    }
</script>
""")

sys.exit(0)
