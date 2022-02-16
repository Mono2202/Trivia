# Imports:
import socket, json

# Constants:
IP = '127.0.0.1'
PORT = 54321
ADDRESS = (IP, PORT)
MSG = 'Hello'
SIZE = 4096
INT_SIZE = 32
BINARY_BASE = 2
BYTE_SIZE = 8
LOGIN_REQUEST_CODE = 101
SIGNUP_REQUEST_CODE = 102
ERROR_REQUEST_CODE = 4
MSG_BYTE_INDEX = 5

# Request chooser function:
def choose_request():
    """
    Getting the user's request
    """
    # User instructions:
    print('Choose a request to send to the server: ')
    print('1 - login request')
    print('2 - signup request')
    print('? - error request')

    # Getting the user's choice:
    choice = input('Choice: ')

    # Condition: login request
    if choice == '1':
        return login_request_builder()
    
    # Condition: signup request
    elif choice == '2':
        return signup_request_builder()
    
    # Condition: error request
    else:
        request = bytearray()
        request.append(ERROR_REQUEST_CODE)
        return request

# Login request builder function:
def login_request_builder():
    """
    Building the user's login request
    """
    # Creating the data:
    data = {}
    data["Username"] = input('Insert username: ')
    data["Password"] = input('Insert password: ')

    # Converting the data to JSON format:
    json_data = json.dumps(data)
    json_data_length = "{0:b}".format(len(json_data)).zfill(INT_SIZE)

    # Building the login request:
    login_request = bytearray()
    login_request.append(LOGIN_REQUEST_CODE)
    login_request.extend(int(json_data_length, BINARY_BASE).to_bytes(len(json_data_length) // BYTE_SIZE, byteorder='big'))
    login_request.extend(json_data.encode())
    return login_request

# Signup request builder function:
def signup_request_builder():
    """
    Building the user's signup request
    """
    # Creating the data:
    data = {}
    data["Username"] = input('Insert username: ')
    data["Password"] = input('Insert password: ')
    data["Email"] = input('Insert email: ')

    # Converting the data to JSON format:
    json_data = json.dumps(data)
    json_data_length = "{0:b}".format(len(json_data)).zfill(INT_SIZE)

    # Building the login request:
    signup_request = bytearray()
    signup_request.append(SIGNUP_REQUEST_CODE)
    signup_request.extend(int(json_data_length, BINARY_BASE).to_bytes(len(json_data_length) // BYTE_SIZE, byteorder='big'))
    signup_request.extend(json_data.encode())
    return signup_request

# main function:
def main():
    # Trying to establish a socket:
    try:
        # Creating the socket:
        server_socket = socket.socket()

        # Connecting to the server:
        server_socket.connect(ADDRESS)
        
        # Getting the user's request to send to the server:
        request = choose_request()

        # Sending the request to the server:
        server_socket.send(request)

        # Receiving data from the server:
       
        server_data = server_socket.recv(SIZE)
        print('Received Message:', server_data[MSG_BYTE_INDEX:].decode())

        # Closing the socket:
        server_socket.close()

    # Catching Exceptions:
    except Exception as e:
        # Printing the Exception:
        print('ERROR:', e)

# Calling main function:
if __name__ == '__main__':
    main()