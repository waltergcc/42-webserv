import os, sys, cgi, datetime

method = os.environ['REQUEST_METHOD']

now = datetime.datetime.now()
formatted = now.strftime("%Y-%m-%d %H:%M:%S")

if method == 'GET':
    print("<h1>Get Page</h1>")
    print("<p><b>Current Time:</b> %s</p>" % formatted)
    print("<h2 align=\"left\">Print Environment:</h2>")
    print("<ul>")

    for k, v in os.environ.items():
        print("<p align=\"left\"><b>%s:</b> %s</p>" % (k, v))

    print("<p> Go to:")
    print("<a href=\"index.html\">Index</a>")
    print("<a href=\"post.html\">Post</a>")
    print("<a href=\"delete.py\">Delete</a>")
    print("</p>")
    print("</ul>")

sys.exit(0)
