import os, sys, cgi, cgitb

cgitb.enable()

method = os.environ["REQUEST_METHOD"]
upload_dir = os.environ["UPLOAD_PATH"]

if "CONTENT_TYPE" in os.environ:
	content_type = os.environ["CONTENT_TYPE"]
else:
	content_type = "error"

if method == "POST":

	if content_type == "plain/text":
		data = os.environ["PAYLOAD"]
		print(f"<pre>{data}</pre>")
		output = os.path.join(upload_dir, "upload.txt")
		with open(output, "w") as f:
			f.write(data)

		print("        <h1>Upload success</h1>")
	else:
		print("        <h1>Error: Unsupported content type</h1>")


