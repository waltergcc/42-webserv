import os, sys, cgi, cgitb

cgitb.enable()

form = cgi.FieldStorage()
method = os.environ["REQUEST_METHOD"]
upload_dir = os.environ["UPLOAD_PATH"]

if method == "POST":
    if "file" in form:
        file_item = form["file"]
        
        if file_item.file:
            filename = os.path.basename(file_item.filename)
            with open(os.path.join(upload_dir, filename), "wb") as f:
                f.write(file_item.file.read())
            print(f"File {filename} uploaded successfully.")
        else:
            print("Failed to upload file.")
    else:
        print("No file was uploaded.")
