from flask import Flask, request
import requests, json

app = Flask(__name__)

@app.route('/', methods=['GET'])
def index():
    return 'Hello, World!'


@app.route('/default', methods=['GET'])
def default():
    
    #Usem as dicas do projeto arduino para entender como setar as luzes
    #Essa rota eu pensei para fazer com que as luzes fiquem piscando como um padrão
    data = {
	'rua0': '001',
	'rua1': '001',
	'rua2': '001',
	'rua3': '001'
    }

    json_string = json.dumps(data)

    #print(json_string)

    # Envia os dados para outra API o valor do endereço pode mudar de acordo com a rede
    # Mas este é informado pela porta serial do esp32
    requests.post("http://192.168.1.4:80/led", data=json_string)

    return 'default'

     
@app.route('/post', methods=['POST'])
def post():
    # Recebe os dados da requisição POST
    data = request.get_json()

    json_string = json.dumps(data)

    # Envia os dados para outra API
    test = requests.post("http://192.168.1.4:80/led", data=json_string)
    #print(test)

    return 'Setup enviado!'


if __name__ == '__main__':
    app.run(debug=True)