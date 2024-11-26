from flask import Flask, render_template, request, jsonify
import time
import serial

app = Flask(__name__)

# Path to the 45.boba file
BOBA_FILE_PATH = "./boba_files/45.boba"

# Serial Port Configuration
COM_PORT = "COM4"
BAUD_RATE = 9600

@app.route('/')
def index():
    """Render the main page with the text input."""
    # Read the existing content of the 45.boba file
    try:
        with open(BOBA_FILE_PATH, "r", encoding="utf-8") as file:
            content = file.read()
    except FileNotFoundError:
        content = ""

    return render_template("index.html", content=content)


@app.route('/write', methods=['POST'])
def write_file():
    """Write user input to the 45.boba file."""
    data = request.json
    new_content = data.get("content", "")

    try:
        with open(BOBA_FILE_PATH, "w", encoding="utf-8") as file:
            file.write(new_content)
        return jsonify({"status": "success", "message": "File updated successfully!"})
    except Exception as e:
        return jsonify({"status": "error", "message": str(e)})


@app.route('/compile', methods=['POST'])
def compile_and_run():
    """Execute the main function."""
    try:
        ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=1)
        time.sleep(2)

        # Read the command from the file
        with open(BOBA_FILE_PATH, "r", encoding="utf-8") as file:
            command = file.read().strip()

        print(f"Sending command: {command}")
        ser.write(command.encode())
        ser.close()

        return jsonify({"status": "success", "message": f"Command '{command}' sent successfully!"})
    except Exception as e:
        return jsonify({"status": "error", "message": str(e)})


if __name__ == "__main__":
    app.run(debug=True)
