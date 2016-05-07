#include "calc.h"

#include <vector>
#include <limits>
#include <QTextStream>
#include <QPoint>
#include <QStandardPaths>
#include <QFile>
#include <QDebug>

#include "Eigen/Dense"
#include "resistor.h"
#include "source.h"
#include "wire.h"
#include "switch.h"

std::shared_ptr<Calc> Calc::instance=nullptr;

using namespace Eigen;
Calc::Calc()
{

}


std::shared_ptr<Calc> Calc::Instance()
{
    //Return instance pointer; if it not exists, make it
    if(!instance){
        instance = std::shared_ptr<Calc>(new Calc());
    }

    return instance;
}


bool Calc::solveLevel()
{
    //Build a list of all nodes and sort it
    std::list<int> nodes;

    for(auto& v:sources){
        nodes.push_back(v->getNodem());
        nodes.push_back(v->getNodep());
    }
    for(auto& r:resistors){

        nodes.push_back(r->getNode1());
        nodes.push_back(r->getNode2());
    }
    for(auto& s:switches){

        nodes.push_back(s->getNode1());
        nodes.push_back(s->getNode2());
    }
    nodes.sort();
    nodes.unique();

    //Calculate voltage of all nodes
    sol=computeNetwork(nodes.size());

    //Standardize all angles for easy calculations
    correctAngles();

    //Set currents trough all components
    setCurrentsOfResistorsAndSwitches();
    for(auto& w:wires){
        w->setCurrent(std::numeric_limits<float>::infinity());
    }
    setCurrentsOfWires();
    if(!(setCurrentsOfStrayWires()))
        return false;


    return true;

}
bool Calc::addResistor(std::shared_ptr<Resistor>& r)
{
    auto size=resistors.size();
    resistors.push_back(r);
    return(size>resistors.size());
}
bool Calc::addSource(std::shared_ptr<Source>& s)
{
    auto size=sources.size();
    sources.push_back(s);
    return(size>sources.size());
}
bool Calc::addWire(std::shared_ptr<Wire>& w)
{
    auto size=wires.size();
    wires.push_back(w);
    return(size>wires.size());
}
bool Calc::addSwitch(std::shared_ptr<Switch>& s)
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
}
bool Calc::readFile()
{

    wires.clear();
    sources.clear();
    resistors.clear();
    switches.clear();

    //TODO Checken of een file volledig juist is
    //DONE

    QFile * file = new QFile(fileName);
    if (file->open(QIODevice::ReadOnly| QIODevice::Text))
    {
        QTextStream in(file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            if (!line.isEmpty()&&!line.isNull()){

                //Determine what to do, depending on the first character
                switch (line.at(0).toLower().toLatin1())
                {
                case '*':

                    switch(line.at(1).toLower().toLatin1()){

                    case's':
                        if(line.at(2).toLower().toLatin1()=='w' && line.length()>4){
                            if(!process_switch_line(line))
                                return false;
                        }
                        else{
                            for (int i=2;i<line.length();i++){
                                if(line.at(i).toLower().toLatin1()=='j'){
                                    qDebug()<<"start of file, found correct sj start";
                                }
                            }
                        }
                        break;

                    case 'g':
                        //Not used anymore, compability with old files
                        break;

                    case 'w':
                        //Read in Wire
                        if(line.length()>2){
                            if(!process_wire_line(line))
                                return false;
                        }
                        break;
                    case 'c':
                        //Read in Click-goals
                        if(!process_click_line(line))
                            return false;
                        break;
                    case '/':
                        //ignore, for file readability
                        break;
                    case'*':
                        //ignore, for comments
                        break;


                    default:
                        qDebug()<<"something went wrong" <<"\n";
                        return false;
                        break;
                    }

                    break;


                case '\n':
                    //just continue
                    break;

                case ' ':
                    //just continue
                    break;

                case 'r':
                    //Read in resistor
                    if(!process_resistor_line(line))
                        return false;
                    break;

                case 'v':
                    //Read in source
                    if(!process_source_line(line))
                        return false;
                    break;

                case '.':
                    //end of file
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
    for (auto& current: list) {

        QStringList wireParams=current.split(" ",QString::SkipEmptyParts);
        if(wireParams.size() == 8){   //Check for right amount of parameters

            int angle=wireParams.at(0).toInt();
            int x=wireParams.at(1).toInt();
            int y=wireParams.at(2).toInt();
            int node=wireParams.at(3).toInt();
            int length=wireParams.at(4).toInt();
            float val= wireParams.at(5).toFloat();
            int isVoltageGoal =wireParams.at(6).toInt();
            int isCurrentGoal = wireParams.at(7).toInt();

            auto w =std::make_shared<Wire>(val,x,y,angle,length,node,0.0,isVoltageGoal,isCurrentGoal);
            wires.push_back(w);

        }
        else{
            qDebug()<<"Bad wire";
            return false;
        }
    }
    return true;
}
bool Calc::process_resistor_line(QString &lijn)
{

    lijn.replace("r","",Qt::CaseSensitivity::CaseInsensitive); //remove r
    QStringList list=lijn.split(" ",QString::SkipEmptyParts);

    if(list.size()==12){ //Check for right amount of parameters

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

    if(list.size()==5){ //Check for right amount of parameters

        int angle=list.at(0).toInt();
        int x=list.at(1).toInt();
        int y=list.at(2).toInt();
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
bool Calc::process_click_line(QString &lijn)
{

    lijn.replace("*","",Qt::CaseSensitivity::CaseInsensitive); //remove *
    lijn.replace("c","",Qt::CaseSensitivity::CaseInsensitive); //remove c
    QStringList list=lijn.split(" ");

    if(list.size()==2){ //Check for right amount of parameters

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


    //Click goals
    out<<"*c"<<twoStar << " " << threeStar <<"\n";

    //Wires
    out <<"*w\n";
    for (auto& wire:wires){
        out<<"*"<<"w"<< wire->getAngle() <<" "<< wire->getXCoord() << " "<< wire->getYCoord()
          <<" "<< wire->getNode() << " " << wire->getLength() << " " << wire->getValue()<<" "<<wire->getIsVoltageGoal()<< " "<<wire->getIsCurrentGoal() <<"\n";
    }
    out <<"*/w\n";

    //Switches
    out<<"*sw\n";
    for (auto& sw:switches){
        out<<"*"<<"sw"<< sw->getAngle() <<" "<< sw->getXCoord() << " "<< sw->getYCoord()
          <<" "<< sw->getNode1() << " " << sw->getNode2() << "\n";
    }
    out<<"*/sw\n";

    out <<"*/sj\n\n";

    //Resistors
    int i = 1;
    for (auto& res:resistors){
        out<<"R"<< i << " " << res->getNode1()<<" "<<res->getNode2()<<" "<<res->getValue()<< " *sj "
          << res->getXCoord() << " "<< res->getYCoord()<<" "<< res->getAngle()<< " "<< res->getIsAdjustable()<<" "<<res->getBeginValue()
          << " " <<res->getStepSize()<<" */sj\n";
        i++;
    }

    //Sources
    i = 1;
    for (auto& source:sources){
        out<<"V"<<i<<" "<<source->getNodep()<<" "<<source->getNodem()<<" "<<source->getValue()<< "v *sj "
          << source->getXCoord() << " "<< source->getYCoord()<<" "<< source->getAngle() << " "<< source->getIsAdjustable()
          <<" "<<source->getBeginValue()<< " " <<source->getStepSize()<<" */sj\n";
        i++;
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

    //Vector for checking voltage with
    std::vector<std::shared_ptr<Component>> toCheck;
    toCheck.insert( toCheck.end(), wires.begin(), wires.end());
    toCheck.insert(toCheck.end(),swAndR.begin(),swAndR.end());

    //If the lowest voltage isn't at the starting side of the component, turn it around
    for(auto& r:swAndR){
        QPoint p(r->getXCoord(),r->getYCoord());
        int angle = r->getAngle();
        int checkNode1 = -1;
        int checkNode2 = -1;


        for(auto& w: toCheck){
            if(w!=r){
                int xp = w->getXCoord();
                int yp = w->getYCoord();
                int l = w->getLength();
                switch (w->getAngle()) {
                case 1:
                    if(p==QPoint(xp,yp) || p == QPoint(xp+l,yp)){
                        checkNode1 = w->getNode1();
                        checkNode2 = w->getNode2();
                        goto Correct;
                    }

                    break;
                case 2:
                    if(p==QPoint(xp,yp) || p == QPoint(xp,yp+l)){
                        checkNode1 = w->getNode1();
                        checkNode2 = w->getNode2();
                        goto Correct;
                    }

                    break;
                case 3:
                    if(p==QPoint(xp,yp) || p == QPoint(xp-l,yp)){
                        checkNode1 = w->getNode1();
                        checkNode2 = w->getNode2();
                        goto Correct;
                    }

                    break;
                case 4:
                    if(p==QPoint(xp,yp) || p == QPoint(xp,yp-l)){
                        checkNode1 = w->getNode1();
                        checkNode2 = w->getNode2();
                        goto Correct;
                    }

                    break;
                default:
                    break;
                }
            }
        }
Correct:
        if(checkNode1 != -1)
        {
            if(voltageAtNode(checkNode1) == std::max(voltageAtNode(r->getNode1()), voltageAtNode(r->getNode2())) || voltageAtNode(checkNode2) == std::max(voltageAtNode(r->getNode1()), voltageAtNode(r->getNode2())) ){
                switch (angle) {
                case 1:
                    r->setAngle(3);
                    r->setXCoord(r->getXCoord() + 1);
                    break;
                case 2:
                    r->setAngle(4);
                    r->setYCoord(r->getYCoord() + 1);
                    break;
                case 3:
                    r->setAngle(1);
                    r->setXCoord(r->getXCoord() - 1);
                    break;
                case 4 :
                    r->setAngle(2);
                    r->setYCoord(r->getYCoord() - 1);
                    break;

                }
            }

        }

    }
}
void Calc::setCurrentsOfResistorsAndSwitches()
{
    for(auto& r : resistors){
        r->setCurrent(std::abs(voltageAtNode(r->getNode1())-voltageAtNode(r->getNode2()))/r->getValue());
    }
    for(auto& sw : switches){
        if(!sw->getUp())
            sw->setCurrent(std::abs(voltageAtNode(sw->getNode1())-voltageAtNode(sw->getNode2()))/sw->getValue());
        else
            sw->setCurrent(0);
    }

}
void Calc::setCurrentsOfWires()
{
    //Joined vector with resistors,switches and sources
    std::vector<std::shared_ptr<Component>> comps;
    comps.insert(comps.end(), resistors.begin(), resistors.end());
    comps.insert(comps.end(), switches.begin(), switches.end());
    comps.insert(comps.end(), sources.begin(),sources.end());


    //For every component, calculate current trough neighbours
    for(auto& c : comps){

        int nodemin,nodemax;
        QPoint pos(c->getXCoord(),c->getYCoord());
        std::shared_ptr<Wire> wTemp;
        std::shared_ptr<Wire> lastWire;
        int connectedWires = 0;
        bool cross = false;
        int node;
        int corFactor = 1;

        //Pointer used for checking if component is a source. Becauce sources behave a bit different
        std::shared_ptr<Source> s = std::dynamic_pointer_cast<Source>(c);

        //Calculate max and min node
        if (s.get()==nullptr){
            if( std::max(voltageAtNode(c->getNode1()), voltageAtNode(c->getNode2())) == voltageAtNode(c->getNode1())){
                nodemax = c->getNode1();
                nodemin = c->getNode2();
            }
            else{
                nodemax = c->getNode2();
                nodemin = c->getNode1();
            }
        }
        else{

            nodemax = c->getNodep();
            nodemin = c->getNodem();
        }



        //Go trough the loop twice, once for maxNode, once for minNode.
        for(int i = 0; i<2;i++){
            switch(i){
            case 0:
                node = nodemin;
                break;

            case 1:
                node = nodemax;
                corFactor *= -1;
                break;

            }


            //Connect connected elements, then assign currents
            while(!cross){
                for(auto& w : wires){
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
                            }
                        }
                    }
                }

                for(auto& co:comps){
                    if(co!=c){
                        if(std::dynamic_pointer_cast<Source>(co)!=nullptr){

                            if(pos == QPoint(co->getXCoord(),co->getYCoord()))
                                connectedWires+=2;
                        }
                        else if(co->getNode1()==node || co->getNode2()==node){
                            int xp = co->getXCoord();
                            int yp = co->getYCoord();
                            switch(co->getAngle()){

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

                //Check nr of connections, if only one: assign current, and jump to the end of the wire
                //else stop calculations for this node
                if(connectedWires ==1){
                    wTemp->setCurrent(c->getCurrent()*corFactor);
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


            //Re-adjust parameters for second loop
            cross = false;
            connectedWires = 0;
            lastWire = nullptr;
            if(s.get()==nullptr){
                int angle= c->getAngle();
                switch(angle){
                case 1:
                    pos.setX(c->getXCoord()+1);
                    pos.setY(c->getYCoord());
                    break;
                case 2:
                    pos.setX(c->getXCoord());
                    pos.setY(c->getYCoord()+1);
                    break;
                case 3:
                    pos.setX(c->getXCoord()-1);
                    pos.setY(c->getYCoord());
                    break;
                case 4:
                    pos.setX(c->getXCoord());
                    pos.setY(c->getYCoord()-1);
                    break;

                }
            }
            else{
                pos.setX(c->getXCoord());
                pos.setY(c->getYCoord());
            }
        }

    }
}

bool Calc::setCurrentsOfStrayWires(){

    int timeout;
    std::vector<std::shared_ptr<Wire>> strayWires;
    std::vector<std::shared_ptr<Wire>> toRemove;

    //Check for wires wich haven't got a real current value and put them in a temp vector
    for(auto& wi:wires){
        if(std::isinf(wi->getCurrent())){
            strayWires.push_back(wi);
        }
    }

    std::vector<std::shared_ptr<Component>> toCheck;
    toCheck.insert(toCheck.end(), wires.begin(),wires.end());
    toCheck.insert(toCheck.end(), resistors.begin(), resistors.end());
    toCheck.insert(toCheck.end(), switches.begin(), switches.end());




    //As long as there are stray wires, stay in loop
    while(!(strayWires.empty())){
        timeout++;

        for(auto& w:strayWires){


            QPoint pos(w->getXCoord(),w->getYCoord());
            int corr = -1;
            if(timeout%2 ==0){
                corr = 1;
                switch(w->getAngle()){
                case 1:
                    pos.setX(pos.x()+1);
                    break;
                case 2:
                    pos.setY(pos.y()+1);
                    break;
                case 3:
                    pos.setX(pos.x()-1);

                    break;
                case 4:
                    pos.setY(pos.y()-1);
                    break;
                default:
                    break;

                }
            }


            float curr = 0;

            //Sum up the currents of al neighbouring wires
            for (auto wire:toCheck){
                if(wire!=w){
                    int xp = wire->getXCoord();
                    int yp = wire->getYCoord();
                    int l = wire->getLength();

                    float current = wire->getCurrent();
                    if(std::dynamic_pointer_cast<Resistor>(wire)){
                        current=current*-1;
                    }
                    switch (wire->getAngle()) {

                    case 1:
                        if(pos==QPoint(xp,yp) ){
                            curr += current;
                        }
                        else if(pos == QPoint(xp+l,yp)){
                            curr -= current;
                        }

                        break;
                    case 2:
                        if(pos==QPoint(xp,yp)){
                            curr += current;
                        }
                        else if (pos== QPoint(xp,yp+l)){
                            curr -= current;
                        }

                        break;
                    case 3:
                        if(pos==QPoint(xp,yp)){
                            curr += current;
                        }
                        else if ( pos== QPoint(xp-l,yp)){
                            curr -= current;
                        }


                        break;
                    case 4:
                        if(pos==QPoint(xp,yp)){
                            curr += current;
                        }
                        else if ( pos== QPoint(xp,yp-l)){
                            curr -= current;
                        }

                        break;
                    default:
                        break;
                    }


                }

            }

            w->setCurrent(corr*curr);

            //If current is real, push to toRemove
            if(!(std::isinf(w->getCurrent())))
                toRemove.push_back(w);

        }


        //Remove toRemove from staywires
        for(auto& r:toRemove){
            strayWires.erase( std::remove( strayWires.begin(), strayWires.end(), r), strayWires.end() );
        }

        //If loop is stuck, return false (worst case scenario)
        if(timeout>50)
            return false;
    }

    return true;




}



std::vector<float> Calc::computeNetwork(int  nrOfNodes)
{
    //Compute voltages for each node, done by matrix calculations. See report for more details

    const int m =sources.size();
    MatrixXf a((nrOfNodes+m),(nrOfNodes+m)); //Matrix with all parameters

    MatrixXf g(nrOfNodes,nrOfNodes); //Part of A Matrix, describes all connected passive elements
    MatrixXf b(nrOfNodes,m);    //Part of A Matrix, describes connections of all sources
    MatrixXf c(m,nrOfNodes);    //Part of A Matrix, transpose of B
    MatrixXf d(m,m);          //All zeros, can be used for dependant sources

    MatrixXf z(nrOfNodes+m,1); // Holds values of independant current and voltage sources
    MatrixXf i(nrOfNodes,1);   //Part of Z matrix, holds values of current sources
    MatrixXf e(m,1); //Part of Z matrix, holds values of voltage sources

    VectorXf x; //Hold unknowm quantities. Voltages at nodes and currents trough source

    //initialize matrices
    g<<MatrixXf::Zero(nrOfNodes,nrOfNodes);
    b<<MatrixXf::Zero(nrOfNodes,m);
    c<<MatrixXf::Zero(m,nrOfNodes);
    d<<MatrixXf::Zero(m,m);
    a<<MatrixXf::Zero(nrOfNodes+m,nrOfNodes+m);
    z<<MatrixXf::Zero(nrOfNodes+m,1);
    i<<MatrixXf::Zero(nrOfNodes,1);
    e<<MatrixXf::Zero(m,1);


    //Joined vector with resistors and switches
    std::vector<std::shared_ptr<Component>> swAndR;
    swAndR.insert( swAndR.end(), resistors.begin(), resistors.end());
    swAndR.insert( swAndR.end(), switches.begin(), switches.end());

    //Fill matrix for g with Resistors and switches
    for(auto& res:swAndR){

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

    //Build A Matrix
    a.resize(g.rows()+c.rows(),b.cols()+g.cols());
    a<<g,b,
            c,d ;




    //Fill e matrix
    for(int i=0;i<m;i++){
        e(i,0)=sources.at(i)->getValue();
    }

    //Build z Matrix
    z<<i,
            e;

    //Solve for X
    x = a.colPivHouseholderQr().solve(z);


    //Save solutions
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



