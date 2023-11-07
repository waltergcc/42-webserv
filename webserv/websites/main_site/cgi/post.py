import os, sys, cgi, cgitb

cgitb.enable()

form = cgi.FieldStorage()
method = os.environ["REQUEST_METHOD"]
upload_dir = os.environ["UPLOAD_PATH"]

if method == "POST":
    print("<h1>Post Page</h1>")
    print("<ul>")
    if "file" in form:
        file_item = form["file"]
        
        if file_item.file:
            filename = os.path.basename(file_item.filename)
            with open(os.path.join(upload_dir, filename), "wb") as f:
                f.write(file_item.file.read())
            print(f"<p>File '<b>{filename}</b>' uploaded successfully.</p>")
        else:
            print("<p>Failed to upload file.</p>")
    else:
        print("<p>No file was uploaded.</p>")

    print("<p> Upload another file? <a href=\"post.html\">Post</a></p>")

    print("<p> Go to:")
    print("<a href=\"index.html\">Index</a>")
    print("<a href=\"get.py\">Get</a>")
    print("<a href=\"delete.py\">Delete</a>")
    print("</p>")

    print("</ul>")
