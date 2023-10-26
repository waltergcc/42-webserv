import os, sys, cgi, datetime

method = os.environ['REQUEST_METHOD']

if method == 'POST':
    print("<h1>Post Page</h1>")
    print("<p>This page is the Get page.</p>")
    print("<a href=\"index.html\">Back to Home</a>")
    print("<h2 align=\"left\">Print Environment:</h2>")
    print("<ul>")

    for k, v in os.environ.items():
        print("<p align=\"left\"><b>%s:</b> %s</p>" % (k, v))

    now = datetime.datetime.now()
    formatted = now.strftime("%Y-%m-%d %H:%M:%S")
    print("<br><p><b>Current Time:</b> %s</p>" % formatted)

    print("</ul>")

sys.exit(0)
