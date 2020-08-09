from socket import *


def main():
    tcp_client_socket = socket(AF_INET, SOCK_STREAM)
    tcp_client_socket.connect(("127.0.0.1", 8888))
    while True:
        meg = input("请输入要发送的消息：")
        tcp_client_socket.send(meg.encode())
        recv_data = tcp_client_socket.recv(1024)

        if recv_data:
            print("返回的消息为:", recv_data.decode('gbk'))
        else:
            print("对方已离线。。")
            break

    tcp_client_socket.close()


if __name__ == '__main__':
    main()
