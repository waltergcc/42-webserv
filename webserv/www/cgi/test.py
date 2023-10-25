import os, sys, cgi

method = os.environ['REQUEST_METHOD']

if method == 'GET':
    print("<h1>GET Method Script</h1>")
    print("<h2>Environment Variables</h2>")
    print("<ul>")

    for k, v in os.environ.items():
        print("<li style='text-align: left;'><b>%s:</b> %s</li>" % (k, v))


    print("</ul>")

sys.exit(0)
