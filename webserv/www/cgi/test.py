import os, sys, cgi, datetime

method = os.environ['REQUEST_METHOD']

if method == 'GET':
    print("<h1>GET Method Script</h1>")
    print("<h2>Environment Variables</h2>")
    print("<ul>")

    for k, v in os.environ.items():
        print("<p><b>%s:</b> %s</p>" % (k, v))

    now = datetime.datetime.now()
    formatted = now.strftime("%Y-%m-%d %H:%M:%S")
    print("<br><p><b>Current Time:</b> %s</p>" % formatted)

    print("</ul>")

sys.exit(0)
