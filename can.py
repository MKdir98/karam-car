from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/receive_can_data', methods=['POST'])
def receive_can_data():
    try:
        data = request.get_json()
        print("Received CAN Data:", data)
        # در اینجا می‌توانید داده‌های دریافتی را به دلخواه پردازش کنید

        return jsonify({'status': 'success'})
    except Exception as e:
        print("Error processing CAN Data:", str(e))
        return jsonify({'status': 'error'})

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
