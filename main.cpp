#include <QCoreApplication>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QByteArray>
#include <QFile>
#include <iostream>
#include <vector>


QJsonObject create_json_object(std::string xjson_input)
{
    QString qsJsonArgs( xjson_input.c_str() );
    QByteArray baJsonArgs = qsJsonArgs.toLocal8Bit();
    QJsonParseError json_error;
    QJsonDocument json_doc = QJsonDocument::fromJson( baJsonArgs,&json_error );

    if ( json_error.error != QJsonParseError::NoError ){
        QString qsError = "Error: invalid json data: ";
        qsError += json_error.errorString();
        throw std::invalid_argument( qsError.toStdString() );
    } // if ( json_error.error != QJsonParseError::NoError ){

    if ( json_doc.isNull() || !json_doc.isObject() ){
        QString qsError = "Error: invalid json object: ";
        qsError += json_error.errorString();
        throw std::invalid_argument( qsError.toStdString() );
    } // if ( json_doc.isNull() || !json_doc.isObject() ){

    return json_doc.object();
}

std::string create_input()
{
    std::string jsonResult;

    QJsonObject joResponse;//root object

    joResponse["status"] = QString("Ok");//(1)
    joResponse["message"] = QString("Action done!");//(1)

    QJsonArray data;//(2)
    joResponse["count"] = 0.25;

    QJsonArray rows;
    for ( int i = 1; i <= 10; i++ )
    {
        QJsonArray cols;
        for ( int j = 1; j <= 10; j++ )
        {
            cols.append( j );
        }
        rows.append( cols );
    }
    joResponse["array"] = rows;

    jsonResult = QJsonDocument(joResponse).toJson(QJsonDocument::Compact).toStdString();

    return  jsonResult;
}

void parse_json(std::string xjson_input)
{
    std::string status = "";
    int count = 0;


    QJsonObject json_obj = create_json_object(xjson_input);

    QJsonArray array;
    array = json_obj["array"].toArray();

    std::vector<std::vector<int>> M(array.size(), std::vector<int>(array[0].toArray().size()));

    for(int i = 0; i < array.size() ; i++)
    {
        QJsonArray col = array[i].toArray();
        for(int j = 0; j < col.size(); j ++)
        {
             M[i][j] = col[j].toInt();// << " ";
             std::cout << M[i][j] << " ";
        }
        std::cout << "\n";
    }

    double count_double =  json_obj["count"].toDouble();
    std::cout << " Impressão de um double :" << count_double;
}

int main(int argc, char *argv[])
{

   QCoreApplication a(argc, argv);


   std::string jsonResult = create_input();
   std::cout << jsonResult;
   parse_json(jsonResult);

    return a.exec();
}
