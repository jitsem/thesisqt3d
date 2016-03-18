#include "calc.h"

#include <iostream>
#include <vector>
#include <limits>

#include <QTextStream>
#include <QPoint>
#include <QStandardPaths>
#include <QDebug>
#include <QFile>


#include "Eigen/Dense"

std::shared_ptr<Calc> Calc::instance=nullptr;

using namespace Eigen;
Calc::Calc()
{

}


std::shared_ptr<Calc> Calc::Instance()
{
    if(!instance){
        instance = std::shared_ptr<Calc>(new Calc());
    }

    return instance;
}


bool Calc::solveLevel()
{
    std::list<int> nodes;

    for(auto v:sources){
        nodes.push_back(v->getNodem());
        nodes.push_back(v->getNodep());
    }
    for(auto r:resistors){

        nodes.push_back(r->getNode1());
        nodes.push_back(r->getNode2());
    }
    for(auto s:switches){

        nodes.push_back(s->getNode1());
        nodes.push_back(s->getNode2());
    }

    nodes.sort();
    nodes.unique();
    sol=computeNetwork(nodes.size());
    correctAngles();
    setCurrentsOfResistorsAndSwitches();

    for(auto w:wires){
        w->setCurrent(std::numeric_limits<float>::infinity());
    }

    setCurrentsOfWires();

    //Draden die niet aan weerstand of switch liggen
    if(!(setCurrentsOfStrayWires()))
        return false;


    return true;

}
bool Calc::addResistor(std::shared_ptr<Resistor> r)
{
    auto size=resistors.size();
    resistors.push_back(r);
    return(size>resistors.size());
}

bool Calc::addSource(std::shared_ptr<Source> s)
{
    auto size=sources.size();
    sources.push_back(s);
    return(size>sources.size());
}
bool Calc::addWire(std::shared_ptr<Wire> w)
{
    auto size=wires.size();
    wires.push_back(w);
    return(size>wires.size());
}
bool Calc::addSwitch(std::shared_ptr<Switch> s)
{
    auto size=switches.size();
    switches.push_back(s);
    return(size>switches.size());
}

void Calc::emptyVectors(){
    resistors.clear();
    sources.clear();
    switches.clear();
    wires.clear();
    goals.clear();
}

bool Calc::readFile()
{

    wires.clear();
    sources.clear();
    resistors.clear();
    switches.clear();
    goals.clear();

    //TODO Checken of een file volledig juist is

    QFile * file = new QFile(fileName);
    if (file->open(QIODevice::ReadOnly| QIODevice::Text))
    {
        QTextStream in(file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            if (!line.isEmpty()&&!line.isNull()){

                switch (line.at(0).toLower().toLatin1()) //to check first character
                {
                case '*':
                    if(line.length()>=2 ){

                        switch(line.at(1).toLower().toLatin1()){

                        case's':
                            if(line.at(2).toLower().toLatin1()=='w' && line.length()>4){
                                if(!process_switch_line(line))
                                    return false;
                            }
                            else{
                                for (int i=2;i<line.length();i++){
                                    if(line.at(i).toLower().toLatin1()=='j'){ //TODO check if index is too big necessary?
                                        qDebug()<<"start of file, found correct sj start";
                                    }
                                }
                            }
                            break;

                        case 'g':
                            if(!process_goal_line(line))
                                return false;
                            break;

                        case 'w':
                            if(line.length()>2){
                                if(!process_wire_line(line))
                                    return false;

                            }
                            break;
                        case 'c':
                            if(!process_click_line(line))
                                return false;
                            break;
                        case '/':
                            if(line.length()>2){
                                //qDebug()<<line;
                                //#ingnore
                            }

                            break;
                        case'*':
                            //ignore, for comments
                            break;


                        default:
                            qDebug()<<"something went wrong" <<"\n";
                            return false;
                            break;
                        }
                    }
                    break;


                case '\n':
                    //just continue
                    //qDebug()<<"read a newline char"<<"\n ";
                    break;

                case ' ':
                    //just continue
                    // qDebug()<<"read a space"<<"\n ";
                    break;

                case 'r':
                    //do for resistor
                    if(!process_resistor_line(line))
                        return false;
                    break;

                case 'v':
                    //do for source
                    if(!process_source_line(line))
                        return false;
                    break;

                case '.':
                    // do for end
                    break;

                default :
                    qDebug()<<"something went wrong" <<"\n";
                    return false;
                    break;
                }
            }


        }
        file->close();
        return true;
    }
    return false;
}

bool Calc::process_wire_line(QString &lijn)
{
    std::vector<std::shared_ptr<Wire>> wir;
    lijn.replace("*","",Qt::CaseSensitivity::CaseInsensitive); //remove *
    lijn.replace("w","",Qt::CaseSensitivity::CaseInsensitive); //remove w

    QStringList list=lijn.split(",");
    for (QStringList::iterator it = list.begin(); it != list.end(); ++it) {
        QString current = *it;
        QStringList wireParams=current.split(" ",QString::SkipEmptyParts);
        if(wireParams.size() == 7){   //Check for right amount of parameters
            int angle=wireParams.at(0).toInt();
            int x=wireParams.at(1).toInt();
            int y=wireParams.at(2).toInt();
            int node=wireParams.at(3).toInt();
            int length=wireParams.at(4).toInt();
            float val= wireParams.at(5).toFloat();
            int isGoal =wireParams.at(6).toInt();
            auto w =std::make_shared<Wire>(val,x,y,angle,length,node,0.0,isGoal);
            wires.push_back(w);
            return true;
        }
        else{
            qDebug()<<"Bad wire";
            return false;
        }
    }

}
bool Calc::process_resistor_line(QString &lijn)
{

    lijn.replace("r","",Qt::CaseSensitivity::CaseInsensitive); //remove r
    QStringList list=lijn.split(" ",QString::SkipEmptyParts);

    if(list.size()==12){

        int node1=list.at(1).toInt();
        int node2=list.at(2).toInt();
        float v=list.at(3).toFloat();
        int x=list.at(5).toInt();
        int y=list.at(6).toInt();
        int angle=list.at(7).toInt();
        int adjust=list.at(8).toInt();
        float begin=list.at(9).toFloat();
        float stepSize = list.at(10).toFloat();
        auto r =std::make_shared<Resistor>(v,node1,node2,x,y,angle,adjust,begin,stepSize);
        resistors.push_back(r);
        return true;
    }
    else{
        qDebug()<<"Bad resistor";
        return false;
    }
}
bool Calc::process_switch_line(QString &lijn)
{

    lijn.replace("*sw","",Qt::CaseSensitivity::CaseInsensitive); //remove *sw
    QStringList list=lijn.split(" ",QString::SkipEmptyParts);

    if(list.size()==5){
        int x=list.at(1).toInt();
        int y=list.at(2).toInt();
        int angle=list.at(0).toInt();
        int node1=list.at(3).toInt();
        int node2=list.at(4).toInt();
        auto sw =std::make_shared<Switch>(node1,node2,x,y,angle);
        switches.push_back(sw);
        return true;
    }
    else{
        qDebug()<<"Bad switch";
        return false;
    }
}
bool Calc::process_source_line(QString &lijn)
{

    lijn.replace("v","",Qt::CaseSensitivity::CaseInsensitive); //remove v
    QStringList list=lijn.split(" ",QString::SkipEmptyParts);

    if(list.size()==12){

        int nodep=list.at(1).toInt();
        int nodem=list.at(2).toInt();
        float v=list.at(3).toFloat();
        int x=list.at(5).toInt();
        int y=list.at(6).toInt();
        int angle=list.at(7).toInt();
        int adjust=list.at(8).toInt();
        float begin=list.at(9).toFloat();
        float stepSize = list.at(10).toFloat();

        auto s =std::make_shared<Source>(v,nodep,nodem,x,y,angle,adjust,begin,stepSize);
        sources.push_back(s);
        return true;
    }
    else{
        qDebug()<<"Bad source";
        return false;
    }
}

bool Calc::process_goal_line(QString &lijn)
{

    lijn.replace("*","",Qt::CaseSensitivity::CaseInsensitive); //remove *
    lijn.replace("g","",Qt::CaseSensitivity::CaseInsensitive); //remove g
    QStringList list=lijn.split(" ");

    if(list.size()==2){
        int x = list.at(0).toInt();
        int y = list.at(1).toInt();
        int node = list.at(2).toInt();
        auto g = std::make_shared<Goal>(x,y,node);
        goals.push_back(g);
        return true;
    }
    else{
        qDebug()<<"Bad Goal";
        return false;

    }
}

bool Calc::process_click_line(QString &lijn)
{

    lijn.replace("*","",Qt::CaseSensitivity::CaseInsensitive); //remove *
    lijn.replace("c","",Qt::CaseSensitivity::CaseInsensitive); //remove c
    QStringList list=lijn.split(" ");

    if(list.size()==2){
        twoStar = list.at(0).toInt();
        threeStar = list.at(1).toInt();
        return true;
    }
    else{
        qDebug()<<"Bad clickGoal";
        return false;
    }
}

void Calc::writeBackToFile()
{
    //Rebuild file format from vectors in the Calc class

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug()<< "oops";
        return;
    }

    QTextStream out(&file);
    out << "*sj\n";

    //Goals for other team
    for (auto goal:goals){
        out << "*g"<< goal->getGoalCoords().x() << " " << goal->getGoalCoords().y() << " " << goal->getGoalNode()<< "\n";

    }
    //Goals for the other team
    out<<"*c"<<twoStar << " " << threeStar <<"\n";

    //Wires
    out <<"*w\n";
    for (auto wire:wires){
        out<<"*"<<"w"<< wire->getAngle() <<" "<< wire->getXCoord() << " "<< wire->getYCoord()
          <<" "<< wire->getNode() << " " << wire->getLength() << " " << wire->getValue()<<" "<<wire->getIsGoal()<< "\n";
    }
    out <<"*/w\n";

    //Switches
    out<<"*sw\n";
    for (auto sw:switches){
        out<<"*"<<"sw"<< sw->getAngle() <<" "<< sw->getXCoord() << " "<< sw->getYCoord()
          <<" "<< sw->getNode1() << " " << sw->getNode2() << "\n";
    }
    out<<"*/sw\n";



    out <<"*/sj\n\n";

    //TODO naam resistor: bv. R1
    for (auto res:resistors){
        out<<"R1 "<<res->getNode1()<<" "<<res->getNode2()<<" "<<res->getValue()<< " *sj "
          << res->getXCoord() << " "<< res->getYCoord()<<" "<< res->getAngle()<< " "<< res->getIsAdjustable()<<" "<<res->getBeginValue()
          << " " <<res->getStepSize()<<" */sj\n";
    }

    //TODO naam Bron: bv. V1
    for (auto source:sources){
        out<<"V1 "<<source->getNodep()<<" "<<source->getNodem()<<" "<<source->getValue()<< "v *sj "
          << source->getXCoord() << " "<< source->getYCoord()<<" "<< source->getAngle() << " "<< source->getIsAdjustable()
          <<" "<<source->getBeginValue()<< " " <<source->getStepSize()<<" */sj\n";
    }

    out <<".end\n";

    file.close();
}


void Calc::correctAngles()
{
    //Joined vector with resistors and switches
    std::vector<std::shared_ptr<Component>> swAndR;
    swAndR.insert( swAndR.end(), resistors.begin(), resistors.end());
    swAndR.insert( swAndR.end(), switches.begin(), switches.end());

    std::vector<std::shared_ptr<Component>> toCheck;
    toCheck.insert( toCheck.end(), wires.begin(), wires.end());
    toCheck.insert(toCheck.end(),swAndR.begin(),swAndR.end());
    //If the lowest voltage isn't at the starting side of the component, turn it around
    for(auto r:swAndR){
        QPoint p(r->getXCoord(),r->getYCoord());
        int angle = r->getAngle();
        int checkNode1 = -1;
        int checkNode2 = -1;


        switch (angle) {

        case 1:

            for(auto w: toCheck){
                int xp = w->getXCoord();
                int yp = w->getYCoord();
                int l = w->getLength();
                switch (w->getAngle()) {
                case 1:
                    if(p == QPoint(xp+l,yp)){
                        checkNode1 = w->getNode1();
                        checkNode2 = w->getNode2();
                        goto Correct1;
                    }

                    break;
                case 2:
                    if(p==QPoint(xp,yp) || p == QPoint(xp,yp+l)){
                        checkNode1 = w->getNode1();
                        checkNode2 = w->getNode2();
                        goto Correct1;
                    }

                    break;
                case 3:
                    if(p == QPoint(xp,yp)){
                        checkNode1 = w->getNode1();
                        checkNode2 = w->getNode2();
                        goto Correct1;
                    }

                    break;
                case 4:
                    if(p==QPoint(xp,yp) || p == QPoint(xp,yp-l)){
                        checkNode1 = w->getNode1();
                        checkNode2 = w->getNode2();
                        goto Correct1;
                    }

                    break;
                default:
                    break;
                }
            }
Correct1:
            if(checkNode1 != -1)
            {
                if(voltageAtNode(checkNode1) == std::max(voltageAtNode(r->getNode1()), voltageAtNode(r->getNode2())) || voltageAtNode(checkNode2) == std::max(voltageAtNode(r->getNode1()), voltageAtNode(r->getNode2())) ){
                    r->setAngle(3);
                    r->setXCoord(r->getXCoord() + 1);

                }

            }
            break;


        case 2:

            for(auto w:wires){
                int xp = w->getXCoord();
                int yp = w->getYCoord();
                int l = w->getLength();
                switch (w->getAngle()) {
                case 1:
                    if(p==QPoint(xp,yp) || p == QPoint(xp+l,yp)){
                        checkNode1 = w->getNode1();
                        checkNode2 = w->getNode2();
                        goto Correct2;
                    }

                    break;
                case 2:
                    if(p == QPoint(xp,yp+l)){
                        checkNode1 = w->getNode1();
                        checkNode2 = w->getNode2();
                        goto Correct2;
                    }

                    break;
                case 3:
                    if(p==QPoint(xp,yp) || p == QPoint(xp-l,yp)){
                        checkNode1 = w->getNode1();
                        checkNode2 = w->getNode2();
                        goto Correct2;
                    }

                    break;
                case 4:
                    if(p == QPoint(xp,yp)){
                        checkNode1 = w->getNode1();
                        checkNode2 = w->getNode2();
                        goto Correct2;
                    }

                    break;
                default:
                    break;
                }
            }
Correct2:
            if(checkNode1 != -1)
            {
                if(voltageAtNode(checkNode1) == std::max(voltageAtNode(r->getNode1()), voltageAtNode(r->getNode2())) || voltageAtNode(checkNode2) == std::max(voltageAtNode(r->getNode1()), voltageAtNode(r->getNode2())) ){
                    r->setAngle(4);
                    r->setYCoord(r->getYCoord() + 1);

                }

            }
            break;
        case 3:
            for(auto w:wires){
                int xp = w->getXCoord();
                int yp = w->getYCoord();
                int l = w->getLength();
                switch (w->getAngle()) {
                case 1:
                    if(p == QPoint(xp,yp)){
                        checkNode1 = w->getNode1();
                        checkNode2 = w->getNode2();
                        goto Correct3;
                    }

                    break;
                case 2:
                    if(p==QPoint(xp,yp) || p == QPoint(xp,yp+l)){
                        checkNode1 = w->getNode1();
                        checkNode2 = w->getNode2();
                        goto Correct3;
                    }

                    break;
                case 3:
                    if(p == QPoint(xp-l,yp)){
                        checkNode1 = w->getNode1();
                        checkNode2 = w->getNode2();
                        goto Correct3;
                    }

                    break;
                case 4:
                    if(p==QPoint(xp,yp) || p == QPoint(xp,yp-l)){
                        checkNode1 = w->getNode1();
                        checkNode2 = w->getNode2();
                        goto Correct3;
                    }

                    break;
                default:
                    break;
                }
            }
Correct3:
            if(checkNode1 != -1)
            {
                if(voltageAtNode(checkNode1) == std::max(voltageAtNode(r->getNode1()), voltageAtNode(r->getNode2())) || voltageAtNode(checkNode2) == std::max(voltageAtNode(r->getNode1()), voltageAtNode(r->getNode2())) ){
                    r->setAngle(1);
                    r->setXCoord(r->getXCoord() - 1);

                }

            }
            break;
        case 4:
            for(auto w:wires){
                int xp = w->getXCoord();
                int yp = w->getYCoord();
                int l = w->getLength();
                switch (w->getAngle()) {
                case 1:
                    if(p==QPoint(xp,yp) || p == QPoint(xp+l,yp)){
                        checkNode1 = w->getNode1();
                        checkNode2 = w->getNode2();
                        goto Correct4;
                    }

                    break;
                case 2:
                    if(p == QPoint(xp,yp)){
                        checkNode1 = w->getNode1();
                        checkNode2 = w->getNode2();
                        goto Correct4;
                    }

                    break;
                case 3:
                    if(p==QPoint(xp,yp) || p == QPoint(xp-l,yp)){
                        checkNode1 = w->getNode1();
                        checkNode2 = w->getNode2();
                        goto Correct4;
                    }

                    break;
                case 4:
                    if(p == QPoint(xp,yp-l)){
                        checkNode1 = w->getNode1();
                        checkNode2 = w->getNode2();
                        goto Correct4;
                    }

                    break;
                default:
                    break;
                }
            }
Correct4:
            if(checkNode1 != -1)
            {
                if(voltageAtNode(checkNode1) == std::max(voltageAtNode(r->getNode1()), voltageAtNode(r->getNode2())) || voltageAtNode(checkNode2) == std::max(voltageAtNode(r->getNode1()), voltageAtNode(r->getNode2())) ){
                    r->setAngle(2);
                    r->setYCoord(r->getYCoord() - 1);

                }

            }
            break;
        default:
            break;
        }
    }
}

void Calc::setCurrentsOfResistorsAndSwitches()
{
    for(auto r : resistors){
        r->setCurrent(std::abs(voltageAtNode(r->getNode1())-voltageAtNode(r->getNode2()))/r->getValue());
    }
    for(auto sw : switches){
        if(!sw->getUp())
            sw->setCurrent(std::abs(voltageAtNode(sw->getNode1())-voltageAtNode(sw->getNode2()))/sw->getValue());
        else
            sw->setCurrent(0);
    }

}


//TODO ook checken op switches
void Calc::setCurrentsOfWires()
{
    //Joined vector with resistors and switches
    std::vector<std::shared_ptr<Component>> swAndR;
    swAndR.insert( swAndR.end(), resistors.begin(), resistors.end());
    swAndR.insert( swAndR.end(), switches.begin(), switches.end());

    for(auto r : swAndR){

        int nodemin,nodemax;
        if( std::max(voltageAtNode(r->getNode1()), voltageAtNode(r->getNode2())) == voltageAtNode(r->getNode1())){
            nodemax = r->getNode1();
            nodemin = r->getNode2();
        }
        else{
            nodemax = r->getNode2();
            nodemin = r->getNode1();
        }

        QPoint pos(r->getXCoord(),r->getYCoord());
        std::shared_ptr<Wire> wTemp;
        std::shared_ptr<Wire> lastWire;
        int connectedWires = 0;
        bool cross = false;
        int node;
        int corFactor = 1;

        //Ga tweemaal door loop, een keer voor maxNode eenmaal voor minNode.
        for(int i = 0; i<2;i++){
            switch(i){
            case 0:
                node = nodemin;
                break;

            case 1:
                node = nodemax;
                corFactor = -1;
                break;

            }

            //Check connected elements, if only one connected: assign same current to wire)
            while(!cross){
                for(auto w : wires){
                    if(w->getNode() == node){
                        if(w!=lastWire){

                            int xp = w->getXCoord();
                            int yp = w->getYCoord();
                            int l = w->getLength();
                            switch (w->getAngle()) {

                            case 1:
                                if(pos==QPoint(xp,yp) || pos == QPoint(xp+l,yp)){
                                    connectedWires++;
                                    wTemp = w;
                                }

                                break;
                            case 2:
                                if(pos==QPoint(xp,yp) || pos== QPoint(xp,yp+l)){
                                    connectedWires++;
                                    wTemp = w;
                                }


                                break;
                            case 3:
                                if(pos == QPoint(xp,yp) ||pos == QPoint(xp-l,yp)){
                                    connectedWires++;
                                    wTemp = w;
                                }


                                break;
                            case 4:
                                if(pos==QPoint(xp,yp) || pos == QPoint(xp,yp-l)){
                                    connectedWires++;
                                    wTemp = w;
                                }

                                break;
                            default:
                                break;
                                //TODO catch default
                            }
                        }
                    }
                }
                for(auto s:sources){
                    if(pos == QPoint(s->getXCoord(),s->getYCoord()))
                        connectedWires+=2;
                }
                for(auto res:resistors){

                    if(res!=r){
                        if(res->getNode1()==node || res->getNode2()==node){
                            int xp = res->getXCoord();
                            int yp = res->getYCoord();
                            switch(res->getAngle()){

                            case 1:
                                if(pos==QPoint(xp,yp) || pos == QPoint(xp+1,yp)){
                                    connectedWires+=2;
                                }

                                break;
                            case 2:
                                if(pos==QPoint(xp,yp) || pos== QPoint(xp,yp+1)){
                                    connectedWires+=2;
                                }


                                break;
                            case 3:
                                if(pos == QPoint(xp,yp) ||pos == QPoint(xp-1,yp)){
                                    connectedWires+=2;
                                }


                                break;
                            case 4:
                                if(pos==QPoint(xp,yp) || pos == QPoint(xp,yp-1)){
                                    connectedWires+=2;
                                }

                                break;
                            default:
                                break;

                            }
                        }
                    }
                }
                if(connectedWires ==1){
                    wTemp->setCurrent(r->getCurrent()*corFactor);
                    switch(wTemp->getAngle()){
                    case 1:
                        if(wTemp->getXCoord()==pos.x())
                            pos.setX(pos.x()+wTemp->getLength());
                        else{
                            pos.setX(pos.x()-wTemp->getLength());
                            wTemp->setCurrent(wTemp->getCurrent()*-1);
                        }
                        break;
                    case 2:
                        if(wTemp->getYCoord()==pos.y())
                            pos.setY(pos.y()+wTemp->getLength());
                        else{
                            pos.setY(+pos.y()-wTemp->getLength());
                            wTemp->setCurrent(wTemp->getCurrent()*-1);
                        }
                        break;
                    case 3:
                        if(wTemp->getXCoord()==pos.x())
                            pos.setX(pos.x()-wTemp->getLength());
                        else{
                            pos.setX(pos.x()+wTemp->getLength());
                            wTemp->setCurrent(wTemp->getCurrent()*-1);
                        }
                        break;
                    case 4:
                        if(wTemp->getYCoord()==pos.y())
                            pos.setY(pos.y()-wTemp->getLength());
                        else{
                            pos.setY(pos.y()+wTemp->getLength());
                            wTemp->setCurrent(wTemp->getCurrent()*-1);
                        }
                        break;
                    }
                    lastWire = wTemp;
                    connectedWires = 0;
                }
                else{
                    cross = true;
                }
            }
            cross = false;
            connectedWires = 0;
            lastWire = nullptr;
            int angle= r->getAngle();
            switch(angle){
            case 1:
                pos.setX(r->getXCoord()+1);
                pos.setY(r->getYCoord());
                break;
            case 2:
                pos.setX(r->getXCoord());
                pos.setY(r->getYCoord()+1);
                break;
            case 3:
                pos.setX(r->getXCoord()-1);
                pos.setY(r->getYCoord());
                break;
            case 4:
                pos.setX(r->getXCoord());
                pos.setY(r->getYCoord()-1);
                break;

            }
        }
    }




}

bool Calc::setCurrentsOfStrayWires(){

    //TODO testen of nog altijd juist
    int timeout;

    std::vector<std::shared_ptr<Wire>> strayWires;
    std::vector<std::shared_ptr<Wire>> toRemove;
    for(auto wi:wires){
        if(std::isinf(wi->getCurrent())){
            strayWires.push_back(wi);


        }
    }


    //Zolang er er nog een weerstand met oneindigestromen is, in while blijven
    while(!(strayWires.empty())){
        timeout++;
        //Check alle draden waar nog geen stroom aan toegekend is
        for(auto w:strayWires){
            if(std::isinf(w->getCurrent())){
                QPoint pos(w->getXCoord(),w->getYCoord());



                float curr = 0;

                //Tel de stromen op van alle andere verbonde draden
                for (auto wire:wires){
                    if(wire!=w){
                        int xp = wire->getXCoord();
                        int yp = wire->getYCoord();
                        int l = wire->getLength();
                        switch (wire->getAngle()) {

                        case 1:
                            if(pos==QPoint(xp,yp) ){
                                curr += wire->getCurrent();
                            }
                            else if(pos == QPoint(xp+l,yp)){
                                curr -= wire->getCurrent();
                            }

                            break;
                        case 2:
                            if(pos==QPoint(xp,yp)){
                                curr += wire->getCurrent();
                            }
                            else if (pos== QPoint(xp,yp+l)){
                                curr -= wire->getCurrent();
                            }

                            break;
                        case 3:
                            if(pos==QPoint(xp,yp)){
                                curr += wire->getCurrent();
                            }
                            else if ( pos== QPoint(xp-l,yp)){
                                curr -= wire->getCurrent();
                            }


                            break;
                        case 4:
                            if(pos==QPoint(xp,yp)){
                                curr += wire->getCurrent();
                            }
                            else if ( pos== QPoint(xp,yp-l)){
                                curr -= wire->getCurrent();
                            }

                            break;
                        default:
                            break;
                        }


                    }

                }

                w->setCurrent(-curr);

                if(!(std::isinf(w->getCurrent())))
                    toRemove.push_back(w);

            }

        }
        for(auto r:toRemove){
            strayWires.erase( std::remove( strayWires.begin(), strayWires.end(), r), strayWires.end() );
        }
        if(timeout>15)
            return false;
    }

    return true;




}



std::vector<float> Calc::computeNetwork(int  nrOfNodes)
{
    //m is nrOfSources
    const int m =sources.size();
    MatrixXf g(nrOfNodes,nrOfNodes);
    MatrixXf b(nrOfNodes,m);
    MatrixXf c(m,nrOfNodes);
    MatrixXf d(m,m);          //dependant sources
    MatrixXf a((nrOfNodes+m),(nrOfNodes+m));

    MatrixXf z(nrOfNodes+m,1);
    MatrixXf i(nrOfNodes,1);
    MatrixXf e(m,1);

    //init matrices
    g<<MatrixXf::Zero(nrOfNodes,nrOfNodes);
    b<<MatrixXf::Zero(nrOfNodes,m);
    c<<MatrixXf::Zero(m,nrOfNodes);
    d<<MatrixXf::Zero(m,m);
    a<<MatrixXf::Zero(nrOfNodes+m,nrOfNodes+m);
    z<<MatrixXf::Zero(nrOfNodes+m,1);
    i<<MatrixXf::Zero(nrOfNodes,1);
    e<<MatrixXf::Zero(m,1);
    //Fill matrix for G

    //Resistors
    for(auto res:resistors){

        for (int i=1;i<=nrOfNodes;i++){
            if(res->getNode1()==i||res->getNode2()==i){
                g(i-1,i-1)+=(1/(res->getValue()));
            }
        }
        if(res->getNode1()!=0&&res->getNode2()!=0){
            g(res->getNode1()-1,res->getNode2()-1)+= (-1/res->getValue());
            g(res->getNode2()-1,res->getNode1()-1)+= (-1/res->getValue());
        }

    }
    //Switches
    for(auto sw:switches){

        for (int i=1;i<=nrOfNodes;i++){
            if(sw->getNode1()==i||sw->getNode2()==i){
                g(i-1,i-1)+=(1/(sw->getValue()));
            }
        }
        if(sw->getNode1()!=0&&sw->getNode2()!=0){
            g(sw->getNode1()-1,sw->getNode2()-1)+= (-1/sw->getValue());
            g(sw->getNode2()-1,sw->getNode1()-1)+= (-1/sw->getValue());
        }

    }


    //Fill matrix b
    for(int i=0;i<m;i++){
        for (int j=1;j<=nrOfNodes;j++){
            if(sources.at(i)->getNodep()==j){
                b(j-1,i)=1;
            }
            if(sources.at(i)->getNodem()==j){
                b(j-1,i)=-1;
            }

        }
    }

    //Fill matrix c
    for(int i=0;i<m;i++){
        for (int j=1;j<=nrOfNodes;j++){
            if(sources.at(i)->getNodep()==j){
                c(i,j-1)=1;
            }
            if(sources.at(i)->getNodem()==j){
                c(i,j-1)=-1;
            }
        }
    }

    a.resize(g.rows()+c.rows(),b.cols()+g.cols());
    a<<g,b,
            c,d       ;




    //fill e matrix
    for(int i=0;i<m;i++){
        e(i,0)=sources.at(i)->getValue();
    }


    z<<i,
            e;

    VectorXf x = a.colPivHouseholderQr().solve(z);

    std::vector<float> solu;
    solu.push_back(0);   //Add value of ground node, always 0
    for (int i=0;i<nrOfNodes-1;i++){
        solu.push_back(x(i));
    }

    for (int i = 0; i< m;i++){
        sources.at(i)->setCurrent(x(i+nrOfNodes));
    }


    return solu;




}

std::vector<std::shared_ptr<Goal> > Calc::getGoals() const
{
    return goals;
}

bool Calc::addGoal(std::shared_ptr<Goal> g)
{
    auto size=goals.size();
    goals.push_back(g);
    return(size>goals.size());
}

int Calc::getThreeStar() const
{
    return threeStar;
}

void Calc::setThreeStar(int value)
{
    threeStar = value;
}

int Calc::getTwoStar() const
{
    return twoStar;
}

void Calc::setTwoStar(int value)
{
    twoStar = value;
}

std::vector<std::shared_ptr<Switch> > Calc::getSwitches() const
{
    return switches;
}

std::vector<std::shared_ptr<Source> > Calc::getSources() const
{
    return sources;
}

std::vector<std::shared_ptr<Resistor> > Calc::getResistors() const
{
    return resistors;
}

std::vector<std::shared_ptr<Wire> > Calc::getWires() const
{
    return wires;
}

QString Calc::getFileName() const
{
    return fileName;
}

void Calc::setFileName(const QString &value)
{
    fileName = value;
}



