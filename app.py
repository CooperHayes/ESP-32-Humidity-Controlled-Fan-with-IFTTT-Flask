from flask import Flask, request, jsonify, render_template
from flask_sqlalchemy import SQLAlchemy

app = Flask(__name__)

app.config["SQLALCHEMY_DATABASE_URI"] = "sqlite:///database.db"
app.config["SQLALCHEMY_TRACK_MODIFICATIONS"] = False

db = SQLAlchemy(app)


class SensorData(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    timestamp = db.Column(db.DateTime,
                          server_default=db.func.now())
    temperature = db.Column(db.Float)
    humidity = db.Column(db.Float)
    threshold = db.Column(db.Float)
    fan = db.Column(db.Boolean)
    


with app.app_context():
    db.create_all()




@app.route("/log", methods=["POST"])
def log():

    data = request.get_json()

    reading = SensorData(
        temperature=data["temperature"],
        humidity=data["humidity"],
        threshold=data["threshold"],
        fan=data["fan"],
    )

    db.session.add(reading)
    db.session.commit()

    return jsonify({"status": "success"}), 200


@app.route("/data")
def data():

    readings = SensorData.query.all()

    output = []

    for r in readings:
        output.append({
            "time": r.timestamp,
            "temperature": r.temperature,
            "humidity": r.humidity,
            "threshold": r.threshold,
            "fan": r.fan,
        })

    return jsonify(output)


@app.route("/")
def tableDisplay():
    readings = SensorData.query.all()
    return render_template("index.html" , readings=readings )

@app.route("/HW")
def helloWorld():
     return "HELLO WORLD"

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)