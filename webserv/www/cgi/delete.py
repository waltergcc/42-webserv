import os, sys, cgi, datetime

method = os.environ['REQUEST_METHOD']

if method == 'GET':
    print("<h1>Delete Page</h1>")
    print("<p>This page is the delete page.</p>")
    print("<a href=\"index.html\">Back to Home</a>")

root_folder = os.environ['ROOT_FOLDER']
folder = os.path.join(root_folder, 'upload')
files = os.listdir(folder)

if files:
    print("<h2>Files</h2>")
    print("<ul>")
    for file in files:
        print(f"<p>{file} <a href=\"#\" data-file=\"{file}\">Remove</a></p>")
    print("</ul>")
else:
    print("<p>No files in 'upload' folder.</p>")

# JavaScript para lidar com a ação de remoção
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
        fetch(`/home/wcorrea-/workplace/common_core/webserv/webserv/www/upload/${file}`, { method: "DELETE" })
            .then(response => {
                if (response.status === 200) {
                    alert(`File ${file} has been deleted.`);
                    // Atualize a página ou faça outras ações necessárias
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
