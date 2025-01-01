from flask import *
import os

app = Flask(__name__)

global_username = None
users_file = "flask/static/txt/username_passwd.txt"
user_backgrounds = [
    file for file in os.listdir("flask/static/img/") if file.endswith(".jpg")
]
global_bg = None
global_userIndex = 0


@app.route("/")
def login():
    return render_template("login.html")


@app.route("/main", methods=["POST"])
def login_post():
    global global_username
    global global_bg
    global global_userIndex
    username_txt = request.form["username"]
    password_txt = request.form["password"]

    with open(users_file, "r") as file:
        for line in file:
            username, password = line.strip().split(": ")
            if username_txt == username and password_txt == password:
                global_username = username
                global_bg = user_backgrounds[global_userIndex]
                return render_template(
                    "main.html", user=global_username, background=global_bg
                )
            global_userIndex += 1
    return "<b>用户名或密码错误，请返回并重新输入</b>"


@app.route("/main")
def mainhtml():
    global global_username
    global global_bg
    return render_template("main.html", user=global_username, background=global_bg)


@app.route("/game")
def famehtml():
    return render_template("game.html")


if __name__ == "__main__":
    app.run(debug=True)
