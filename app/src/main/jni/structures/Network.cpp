//
// Created by alejandro on 20/10/16.
//

#include "headers/Network.h"
#include <chrono>

using namespace chrono;

Network::Network() {

}

Network::Network(const char * serverIP,const int& serverPort) {
    this->serverIP = serverIP;
    this->serverPort = serverPort;
    this->timer = Timer();
    this->timer.addTime("REMOTE - Sending total");
    this->timer.addTime("REMOTE - Resizing");
    this->timer.addTime("REMOTE - Encoding");
    this->timer.addTime("REMOTE - Sending only");
    this->timer.addTime("REMOTE - Receiving total");
    this->timer.addTime("REMOTE - Receiving only");
    this->timer.addTime("REMOTE - Unserializing");
    this->timer.addTime("REMOTE - Editing");
    this->open = false;
}

bool Network::startConnection() {
    if(!this->open) {
        struct sockaddr_in serverAddress;
        socklen_t addrLength = sizeof(struct sockaddr_in);

        sokt = socket(PF_INET, SOCK_STREAM, 0);

        if (sokt < 0) {
            error("CLIENTE", "Error al iniciar socket");
            return false;
        } else {

            int yes = 1;
            int resultNoDelay = setsockopt(sokt, IPPROTO_TCP, TCP_NODELAY, (char *) &yes, sizeof(int));    // 1 - on, 0 - off
//            int resultQuickAck = setsockopt(sokt, IPPROTO_TCP, TCP_QUICKACK, (char *) &yes, sizeof(int));    // 1 - on, 0 - off

            if(resultNoDelay < 0 /*|| resultQuickAck < 0*/){
                error("CLIENTE", "Error con TCP_NODELAY o TCP_QUICKACK");
                return false;
            } else{
                serverAddress.sin_family = PF_INET;
                serverAddress.sin_addr.s_addr = inet_addr(serverIP);
                serverAddress.sin_port = htons(serverPort);

                if (connect(sokt, (sockaddr *) &serverAddress, addrLength) < 0) {
                    error("CLIENTE", "Error al conectar");
                    this->deepLearning = 0;
                    return false;
                } else {
                    log("CLIENTE", "Conectado al servidor");

                    /* Recibir entero para ver si deep learning o no */

                    this->deepLearning = this->receiveInt() == 1;

                    this->open = true;
                    return true;
                }
            }
        }
    }
}

bool Network::closeConnection() {

    if(open){
        log("CLIENTE", "Cerrando conexion");
        close(sokt);
        this->open = false;
        return true;
    }
}

bool Network::sendImage(const Mat& image) {
    high_resolution_clock::time_point beginSending = high_resolution_clock::now();

    high_resolution_clock::time_point beginEncoding = high_resolution_clock::now();

    Mat imageToSend;
    if(deepLearning){
        error("MODO DEEP LEARNING", "ZI");
        high_resolution_clock::time_point beginResizing = high_resolution_clock::now();

        /* Resizear imagen */
        Size size(256,256);
        resize(image, imageToSend, size);

        /* Ends measuring time */
        high_resolution_clock::time_point endResizing = high_resolution_clock::now();

        /* Added to total time */
        auto durationResizing = duration_cast<microseconds>( endResizing - beginResizing ).count();
        this->timer.addDuration("REMOTE - Resizing", durationResizing);
    } else{
        error("MODO KEY POINT", "ZI");
        imageToSend = image.clone();
    }

    log("CLIENTE", "Preparado para enviar");
    if(!imageToSend.isContinuous()){
        imageToSend = imageToSend.clone();
    }
    int imgSize = imageToSend.total() * imageToSend.elemSize();
    int bytes = 0;
    vector < uchar > encoded;
    vector < int > compression_params;
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(90);

    imencode(".jpg", imageToSend, encoded, compression_params);

    /* Ends measuring time */
    high_resolution_clock::time_point endEncoding = high_resolution_clock::now();

    /* Added to total time */
    auto durationEncoding = duration_cast<microseconds>( endEncoding - beginEncoding ).count();
    this->timer.addDuration("REMOTE - Encoding", durationEncoding);

    log("SIZE", intToString(encoded.size()));

    high_resolution_clock::time_point beginSendingOnly = high_resolution_clock::now();

    if(this->sendInt(encoded.size())){

        log("CLIENTE", "Enviando imagen...");
        bytes = send(sokt, encoded.data(), encoded.size(), 0);
        if(bytes < 0){
            error("CLIENTE", "Error al enviar imagen");
            return false;
        } else{
            /* Ends measuring time */
            high_resolution_clock::time_point endSending = high_resolution_clock::now();

            /* Added to total time */
            auto durationSending = duration_cast<microseconds>( endSending - beginSending ).count();
            auto durationSendingOnly = duration_cast<microseconds>( endSending - beginSendingOnly ).count();
            this->timer.addDuration("REMOTE - Sending total", durationSending);
            this->timer.addDuration("REMOTE - Sending only", durationSendingOnly);
            return true;
        }
    } else{
        return false;
    }


}

bool Network::receiveImage(Mat image, Result& result) {
    high_resolution_clock::time_point beginReceiving = high_resolution_clock::now();

    log("CLIENTE", "Preparado para recibir");
    int receivedInt;
    int bytes = 0;

    if((receivedInt = this->receiveInt()) < 0){
        return false;
    } else{
        string output(receivedInt,0);
//        vector<uchar> receivedEncoded(receivedInt);

        bytes = recv(sokt, &output[0], receivedInt, MSG_WAITALL);
        if(bytes < 0){
            error("CLIENTE", "Error al recibir imagen");
            return false;
        } else{
            log("CLIENTE", "Recibido resultado");

            high_resolution_clock::time_point beginUnserializing = high_resolution_clock::now();

            /* Added to total time */
            auto durationReceivingOnly = duration_cast<microseconds>( beginUnserializing - beginReceiving ).count();
            this->timer.addDuration("REMOTE - Receiving only", durationReceivingOnly);

            Result imgResult = Result();
            bool success = imgResult.deserializeResult(output);

            if(success){

                /* Ends measuring time */
                high_resolution_clock::time_point endUnserializing = high_resolution_clock::now();

                /* Added to total time */
                auto durationUnserializing = duration_cast<microseconds>( endUnserializing - beginUnserializing ).count();
                this->timer.addDuration("REMOTE - Unserializing", durationUnserializing);

                high_resolution_clock::time_point beginEditing = high_resolution_clock::now();

                Scalar colour;
                if(imgResult.getObjectName() != "No object" && imgResult.getCorners().size() == 4){
                    colour = Scalar(0,255,0);
                    line(image, imgResult.getCorners()[0], imgResult.getCorners()[1], colour, 4);
                    line(image, imgResult.getCorners()[1], imgResult.getCorners()[2], colour, 4);
                    line(image, imgResult.getCorners()[2], imgResult.getCorners()[3], colour, 4);
                    line(image, imgResult.getCorners()[3], imgResult.getCorners()[0], colour, 4);
                }


                Size textsize = getTextSize(imgResult.getObjectName(), FONT_HERSHEY_COMPLEX, 1, 2, 0);
                Point org((640 - textsize.width - 20), textsize.height + 20);
                putText( image, imgResult.getObjectName(), org, FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0), 2);

                for(int i = 0; i < imgResult.getAllergens().size(); i++){
                    textsize = getTextSize(imgResult.getAllergens()[i], FONT_HERSHEY_COMPLEX, 0.75, 2, 0);
                    Point org((640 - textsize.width - 20), textsize.height + 20 + 30*(i+1));

                    /* Comprobar con lista de alergenos para pintar de verde o de rojo */
                    putText( image, imgResult.getAllergens()[i], org, FONT_HERSHEY_COMPLEX, 0.75, Scalar(0, 0, 255), 2);
                }

                /* Ends measuring time */
                high_resolution_clock::time_point endEditing = high_resolution_clock::now();

                /* Added to total time */
                auto durationEditing = duration_cast<microseconds>( endEditing - beginEditing ).count();
                this->timer.addDuration("REMOTE - Editing", durationEditing);

//            imdecode(receivedEncoded, CV_LOAD_IMAGE_COLOR).copyTo(image);
                log("CLIENTE", "Imagen recibida correctamente");

                /* Ends measuring time */
                high_resolution_clock::time_point endReceiving = high_resolution_clock::now();

                /* Added to total time */
                auto durationReceiving = duration_cast<microseconds>( endReceiving - beginReceiving ).count();
                this->timer.addDuration("REMOTE - Receiving total", durationReceiving);

                result = imgResult;
                return true;
            } else{
                error("CLIENTE", "Error al deserializar");
                return false;
            }
        }
    }
}

bool Network::sendInt(const int& n) {
    int converted_number = htonl(n);
    int bytes = 0;
    log("CLIENTE", "SIZE: "+ intToString(sizeof(converted_number)));
    bytes = send(sokt, (const char*)&converted_number, sizeof(converted_number), 0);
    if(bytes < 0){
        error("CLIENTE", "Error al enviar entero");
        return false;
    } else{
        log("CLIENTE", "Enviado " + intToString(n));
        return true;
    }
}

int Network::receiveInt() {
    int converted_received_int;
    int received_int;
    int bytes = 0;

    bytes = recv(sokt, &converted_received_int, 4, MSG_WAITALL);
    if(bytes < 0){
        error("CLIENTE", "Error al recibir entero");
        return -1;
    } else{
        received_int = ntohl(converted_received_int);
        log("CLIENTE", "Recibido " + intToString(received_int));
        return  received_int;
    }
}

void Network::printTimer() {
    this->timer.printTimes();
}

bool Network::isInit() const{
    return this->serverPort == 8888 || this->serverPort == 80;
}