import serial
import time

# Configurações da porta serial
SERIAL_PORT = 'COM3'  # Substitua pelo nome da sua porta serial
SERIAL_BAUDRATE = 57600
SERIAL_TIMEOUT = 1  # Tempo de espera para leitura

# threshold para luminosidade
LUMINOSITY_THRESHOLD = 200

def serial_server():
    # Configuração da porta serial
    ser = serial.Serial(SERIAL_PORT, SERIAL_BAUDRATE, timeout=SERIAL_TIMEOUT)
    print(f"Servidor serial iniciado na porta {SERIAL_PORT} com baudrate {SERIAL_BAUDRATE}")

    led_on = False  # Estado atual do LED

    try:
        while True:
            # Lê dados da porta serial
            data = ser.readline().decode('utf-8').strip()
            if data:
                # Obtém o tempo atual
                current_time = time.strftime("%Y-%m-%d %H:%M:%S")
                
                # Imprime o valor recebido e o instante em que foi recebido
                print(f"Recebido: {data} às {current_time}")
                
                # Tenta converter o valor recebido para um número
                try:
                    luminosity_value = float(data)    
                    # Verifica se o valor está abaixo ou acima do limite
                    if luminosity_value < LUMINOSITY_THRESHOLD and not led_on:
                        print("Luminosidade abaixo do limite. Enviando comando para acender o LED.")
                        ser.write(b'LED_ON\n')  # Envia o comando para acender o LED
                        led_on = True
                    elif luminosity_value >= LUMINOSITY_THRESHOLD and led_on:
                        print("Luminosidade acima do limite. Enviando comando para apagar o LED.")
                        ser.write(b'LED_OFF\n')  # Envia o comando para apagar o LED
                        led_on = False
                except ValueError:
                    print("Valor recebido não é um número válido.")
    except KeyboardInterrupt:
        print("Servidor serial encerrado pelo usuário.")
    finally:
        ser.close()

if __name__ == "__main__":
    serial_server()