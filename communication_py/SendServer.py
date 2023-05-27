import os
from flask import Flask, url_for

app = Flask(__name__)


# ルーティング
@app.route("/read")
def read():
    path = "scripts/data.csv"
    f = open(path)
    data = f.read()
    f.close()
    return str(data)


if __name__ == "__main__":
    app.run(port=8000, debug=True)