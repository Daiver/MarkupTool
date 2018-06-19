#include "loaderjsonbody.h"


ShapeFace LoaderJSON::getBody(const QJsonDocument &doc)
{
    QJsonObject obj = doc.object();

    QJsonArray leftEyeCentersArray = obj.value("LeftEyeCenters").toArray();
    QJsonArray leftEyeCornerArray = obj.value("LeftEyeCorners").toArray();
    QJsonArray leftEyeUpArray = obj.value("LeftEyeUp").toArray();
    QJsonArray leftEyeDownArray = obj.value("LeftEyeDown").toArray();
    QJsonArray leftEyeInvisibleObj = obj.value("LeftEyeInvisible").toArray();

    QJsonArray rightEyeCentersArray = obj.value("RightEyeCenters").toArray();
    QJsonArray rightEyeCornerArray = obj.value("RightEyeCorners").toArray();
    QJsonArray rightEyeUpArray = obj.value("RightEyeUp").toArray();
    QJsonArray rightEyeDownArray = obj.value("RightEyeDown").toArray();
    QJsonArray rightEyeInvisibleObj = obj.value("RightEyeInvisible").toArray();

    QJsonArray mouthEyeCentersArray = obj.value("MouthCenters").toArray();
    QJsonArray mouthEyeCornerArray = obj.value("MouthCorners").toArray();
    QJsonArray mouthEyeUpArray = obj.value("MouthUp").toArray();
    QJsonArray mouthEyeDownArray = obj.value("MouthDown").toArray();
    QJsonArray mouthEyeInvisibleObj = obj.value("MouthInvisible").toArray();

    ShapeFace body;
    body.parts[0].centers = LoaderJSON::getPoints(leftEyeCentersArray);
    body.parts[0].corner = LoaderJSON::getPoints(leftEyeCornerArray);
    body.parts[0].up = LoaderJSON::getPoints(leftEyeUpArray);
    body.parts[0].down = LoaderJSON::getPoints(leftEyeDownArray);
    body.parts[0].invisibleSegments = LoaderJSON::getInvisibleSegment(leftEyeInvisibleObj);

    body.parts[1].centers = LoaderJSON::getPoints(rightEyeCentersArray);
    body.parts[1].corner = LoaderJSON::getPoints(rightEyeCornerArray);
    body.parts[1].up = LoaderJSON::getPoints(rightEyeUpArray);
    body.parts[1].down = LoaderJSON::getPoints(rightEyeDownArray);
    body.parts[1].invisibleSegments = LoaderJSON::getInvisibleSegment(rightEyeInvisibleObj);

    body.parts[2].centers = LoaderJSON::getPoints(mouthEyeCentersArray);
    body.parts[2].corner = LoaderJSON::getPoints(mouthEyeCornerArray);
    body.parts[2].up = LoaderJSON::getPoints(mouthEyeUpArray);
    body.parts[2].down = LoaderJSON::getPoints(mouthEyeDownArray);
    body.parts[2].invisibleSegments = LoaderJSON::getInvisibleSegment(mouthEyeInvisibleObj);

    return body;
}



QJsonDocument LoaderJSON::createJson(const ShapeFace *body)
{
    qDebug() << "inside create JSon";
    QJsonObject obj;
    QJsonArray leftEyeCenters = getArray(body->parts[0].centers);
    QJsonArray leftEyeCornerArray = getArray(body->parts[0].corner);
    QJsonArray leftEyeUpArray = getArray(body->parts[0].up);
    QJsonArray leftEyeDownArray = getArray(body->parts[0].down);
    QJsonArray leftEyeInvisibleArray;

    QJsonArray rightEyeCenters = getArray(body->parts[1].centers);
    QJsonArray rightEyeCornerArray = getArray(body->parts[1].corner);
    QJsonArray rightEyeUpArray = getArray(body->parts[1].up);
    QJsonArray rightEyeDownArray = getArray(body->parts[1].down);
    QJsonArray rightEyeInvisibleArray;

    QJsonArray mouthEyeCenters = getArray(body->parts[2].centers);
    QJsonArray mouthEyeCornerArray = getArray(body->parts[2].corner);
    QJsonArray mouthEyeUpArray = getArray(body->parts[2].up);
    QJsonArray mouthEyeDownArray = getArray(body->parts[2].down);
    QJsonArray mouthEyeInvisibleArray;


    QJsonObject objLeftEyeInvisibleArray = getInvisibleSegment(body->parts[0]);
    QJsonObject objRightEyeInvisibleArray = getInvisibleSegment(body->parts[1]);
    QJsonObject objMouthInvisibleArray = getInvisibleSegment(body->parts[2]);

    leftEyeInvisibleArray.push_back(objLeftEyeInvisibleArray);
    rightEyeInvisibleArray.push_back(objRightEyeInvisibleArray);
    mouthEyeInvisibleArray.push_back(objMouthInvisibleArray);

    obj.insert("LeftEyeCenters", leftEyeCenters);
    obj.insert("LeftEyeCorners", leftEyeCornerArray);
    obj.insert("LeftEyeUp", leftEyeUpArray);
    obj.insert("LeftEyeDown", leftEyeDownArray);
    obj.insert("LeftEyeInvisible", leftEyeInvisibleArray);

    obj.insert("RightEyeCenters", rightEyeCenters);
    obj.insert("RightEyeCorners", rightEyeCornerArray);
    obj.insert("RightEyeUp", rightEyeUpArray);
    obj.insert("RightEyeDown", rightEyeDownArray);
    obj.insert("RightEyeInvisible", rightEyeInvisibleArray);

    obj.insert("MouthCenters", mouthEyeCenters);
    obj.insert("MouthCorners", mouthEyeCornerArray);
    obj.insert("MouthUp", mouthEyeUpArray);
    obj.insert("MouthDown", mouthEyeDownArray);
    obj.insert("MouthInvisible", mouthEyeInvisibleArray);

    QJsonDocument doc(obj);
    qDebug() << "Out create JSon";
    return doc;
}



QJsonObject LoaderJSON::getInvisibleSegment(const Segment &segment)
{
    QJsonObject obj;
    for (int indInvisSegm = 0; indInvisSegm < segment.invisibleSegments.size(); indInvisSegm++){
        QJsonArray indexesPointsInSegment = getArray(segment.invisibleSegments[indInvisSegm]);
        obj.insert("Segment " + QString::number(indInvisSegm), indexesPointsInSegment);
    }
    return obj;
}



QList<QVector<int> > LoaderJSON::getInvisibleSegment(QJsonArray &obj)
{
    QList<QVector<int>> invisibleSegments;
    for (int indSegment = 0; indSegment < obj[0].toObject().size(); indSegment++){
       QJsonArray jsonSegment = obj[0].toObject().value("Segment " + QString::number(indSegment)).toArray();
       QVector<int> segment = LoaderJSON::getIndex(jsonSegment);
       invisibleSegments.push_back(segment);
    }
    return invisibleSegments;
}
