import os, sys, cgi, datetime

method = os.environ['REQUEST_METHOD']

now = datetime.datetime.now()
formatted = now.strftime("%Y-%m-%d %H:%M:%S")

if method == 'GET':

    print("<header>")
    print("<nav class=\"navbar\">")
    print("<div class=\"container\">")
    print("<h1>Get Page</h1>")
    print("<ul class=\"navigation\">")
    print("<a class=\"glow-on-hover\" href=\"index.html\">HOME</a>")
    print("<a class=\"glow-on-hover\" href=\"post.html\">POST</a>")
    print("<a class=\"glow-on-hover\" href=\"delete.py\">DELETE</a>")
    print("</ul>")
    print("</div>")
    print("</nav>")
    print("</header>")

    print("<div class=\"content\">")
    print("<p><b>Current Time:</b> %s</p>" % formatted)
    print("<h2 align=\"left\">Print Environment:</h2>")
    print("<ul>")

    for k, v in os.environ.items():
        print("<p align=\"left\"><b>%s:</b> %s</p>" % (k, v))

    print("</ul>")
    print("</div>")

sys.exit(0)
