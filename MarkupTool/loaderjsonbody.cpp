#include "loaderjsonbody.h"


Body LoaderJSON::getBody(const QJsonDocument &doc)
{
    QJsonObject obj = doc.object();

    QJsonArray leftEyeCornerArray = obj.value("LeftEyeCorners").toArray();
    QJsonArray leftEyeUpArray = obj.value("LeftEyeUp").toArray();
    QJsonArray leftEyeDownArray = obj.value("LeftEyeDown").toArray();

    QJsonArray rightEyeCornerArray = obj.value("RightEyeCorners").toArray();
    QJsonArray rightEyeUpArray = obj.value("RightEyeUp").toArray();
    QJsonArray rightEyeDownArray = obj.value("RightEyeDown").toArray();

    QJsonArray mouthEyeCornerArray = obj.value("MouthCorners").toArray();
    QJsonArray mouthEyeUpArray = obj.value("MouthUp").toArray();
    QJsonArray mouthEyeDownArray = obj.value("MouthDown").toArray();

    Body body;
    body.parts[0].corner = LoaderJSON::getPoints(leftEyeCornerArray);
    body.parts[0].up = LoaderJSON::getPoints(leftEyeUpArray);
    body.parts[0].down = LoaderJSON::getPoints(leftEyeDownArray);

    body.parts[1].corner = LoaderJSON::getPoints(rightEyeCornerArray);
    body.parts[1].up = LoaderJSON::getPoints(rightEyeUpArray);
    body.parts[1].down = LoaderJSON::getPoints(rightEyeDownArray);

    body.parts[2].corner = LoaderJSON::getPoints(mouthEyeCornerArray);
    body.parts[2].up = LoaderJSON::getPoints(mouthEyeUpArray);
    body.parts[2].down = LoaderJSON::getPoints(mouthEyeDownArray);

    return body;
}



QJsonDocument LoaderJSON::createJson(const Body *body)
{
    QJsonObject obj;
    QJsonArray leftEyeCornerArray = getArray(body->parts[0].corner);
    QJsonArray leftEyeUpArray = getArray(body->parts[0].up);
    QJsonArray leftEyeDownArray = getArray(body->parts[0].down);

    QJsonArray rightEyeCornerArray = getArray(body->parts[1].corner);
    QJsonArray rightEyeUpArray = getArray(body->parts[1].up);
    QJsonArray rightEyeDownArray = getArray(body->parts[1].down);

    QJsonArray mouthEyeCornerArray = getArray(body->parts[2].corner);
    QJsonArray mouthEyeUpArray = getArray(body->parts[2].up);
    QJsonArray mouthEyeDownArray = getArray(body->parts[2].down);

    obj.insert("LeftEyeCorners", leftEyeCornerArray);
    obj.insert("LeftEyeUp", leftEyeUpArray);
    obj.insert("LeftEyeDown", leftEyeDownArray);

    obj.insert("RightEyeCorners", rightEyeCornerArray);
    obj.insert("RightEyeUp", rightEyeUpArray);
    obj.insert("RightEyeDown", rightEyeDownArray);

    obj.insert("MouthCorners", mouthEyeCornerArray);
    obj.insert("MouthUp", mouthEyeUpArray);
    obj.insert("MouthDown", mouthEyeDownArray);

    QJsonDocument doc(obj);
    return doc;
}
