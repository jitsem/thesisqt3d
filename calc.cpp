#include "calc.h"
#include <iostream>
#include <vector>
#include <QTextStream>
#include "Eigen/Dense"



using namespace Eigen;
Calc::Calc()
{

}

void Calc::solveLevel()
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


    nodes.sort();
    nodes.unique();

    sol=computeNetwork(sources,resistors,nodes.size());

}


void Calc::readFile(QString s)
{
    QFile * file = new QFile(s);

    if (file->open(QIODevice::ReadOnly| QIODevice::Text))
    {
        QTextStream in(file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            if (!line.isEmpty()&&!line.isNull()){


                QChar first =line.at(0).toLower();
                switch (first.toLatin1()) //to check first character
                {
                case '*':{
                    if(line.length()>=2 ){

                        QChar second =line.at(1).toLower().toLatin1();

                        if(second=='s'){
                            for (int i=2;i<line.length();i++){
                                if(line.at(i).toLower().toLatin1()=='j'){ //TODO check if index is too big necessary?
                                    qDebug()<<"start of file, found correct sj start";
                                }
                            }
                            break;//break from * case
                        }
                        else if(second=='g'){
                            //qDebug()<<"found ground, ignored location at the moment";
                            break;
                        }

                        else if(second=='w'){
                            if(line.length()>2){

                                auto wir=process_wire_line(line);
                                wires.insert(std::end(wires), std::begin(wir), std::end(wir));
                            }
                            else{
                                break;
                            }

                        }
                        else if (second=='/'){
                            if(line.length()>2){
                                //qDebug()<<line;
                                //#ingnore
                            }
                        }
                        break;
                    }
                }

                case '\n':{
                    //just continue
                    //qDebug()<<"read a newline char"<<"\n ";
                    break;
                }
                case ' ':{
                    //just continue
                    // qDebug()<<"read a space"<<"\n ";
                    break;
                }
                case 'r':{
                    //do for resistor
                    process_resistor_line(line);
                    break;
                }
                case 'v':{
                    //do for source
                    process_source_line(line);
                    break;
                }
                case '.':{
                    // do for end
                    break;
                }
                default :
                    qDebug()<<"something went wrong" <<"\n";
                }
            }


        }
        file->close();
    }
}

std::vector<std::shared_ptr<Wire> > Calc::process_wire_line(QString &lijn)
{
    std::vector<std::shared_ptr<Wire>> wir;
    lijn.replace("*","",Qt::CaseSensitivity::CaseInsensitive); //remove *
    lijn.replace("w","",Qt::CaseSensitivity::CaseInsensitive); //remove w
    QStringList list=lijn.split(",");
    for (QStringList::iterator it = list.begin(); it != list.end(); ++it) {
        QString current = *it;
        QStringList list2=current.split(" ",QString::SkipEmptyParts);//TODO better name
        //qDebug()<<list2.at(2);
        int x=list2.at(1).toInt();
        int y=list2.at(2).toInt();
        int angle=list2.at(0).toInt();
        int length=list2.at(4).toInt();
        int node=list2.at(3).toInt();
        auto w =std::make_shared<Wire>(x,y,angle,length,node);
        wir.push_back(w); //TODO check if input is correct!!


    }

    return wir;
}

void Calc::process_resistor_line(QString &lijn)
{

    lijn.replace("r","",Qt::CaseSensitivity::CaseInsensitive); //remove r
    QStringList list=lijn.split(" ",QString::SkipEmptyParts);
    //for (QStringList::iterator it = list.begin(); it != list.end(); ++it) {
    //QString current = *it;
    int x=list.at(5).toInt();
    int y=list.at(6).toInt();
    int angle=list.at(7).toInt();
    int node1=list.at(1).toInt();
    int node2=list.at(2).toInt();
    float v=list.at(3).toFloat();
    auto r =std::make_shared<Resistor>(v,node1,node2,x,y,angle);
    resistors.push_back(r); //TODO check if input is correct!!

    //}
    int i=0;

}

void Calc::process_source_line(QString &lijn)
{

    lijn.replace("v","",Qt::CaseSensitivity::CaseInsensitive); //remove v
    QStringList list=lijn.split(" ",QString::SkipEmptyParts);
    int x=list.at(5).toInt();
    int y=list.at(6).toInt();
    int angle=list.at(7).toInt();
    int nodep=list.at(1).toInt();
    int nodem=list.at(2).toInt();
    float v=list.at(3).toFloat();
    auto s =std::make_shared<Source>(v,nodep,nodem,x,y,angle);
    sources.push_back(s); //TODO check if input is correct!!


}



//TODO sources/resistors direct aanspreken
std::vector<float> Calc::computeNetwork(std::vector<std::shared_ptr<Component> > & s, std::vector<std::shared_ptr<Component> > &r, int  nrOfNodes)
{
    //m is nrOfSources
    const int m =s.size();
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
    // TODO make list of nodes and loop trough
    for(std::shared_ptr<Component> res:r){

        for (int i=1;i<=nrOfNodes;i++){
            if(res->getNode1()==i||res->getNode2()==i){
                //std::cout<<(1/(res->getValue()))<<std::endl;
                g(i-1,i-1)+=(1/(res->getValue()));
            }
        }
        if(res->getNode1()!=0&&res->getNode2()!=0){
            g(res->getNode1()-1,res->getNode2()-1)+= (-1/res->getValue());
            g(res->getNode2()-1,res->getNode1()-1)+= (-1/res->getValue());
        }

    }
    //    std::cout<<"matrix g"<<std::endl;
    //    std::cout<<g<<std::endl;

    //Fill matrix b
    for(int i=0;i<s.size();i++){
        for (int j=1;j<=nrOfNodes;j++){
            if(s.at(i)->getNodep()==j){
                b(j-1,i)=1;
            }
            if(s.at(i)->getNodem()==j){
                b(j-1,i)=-1;
            }

        }
    }
    //    std::cout<<"matrix b"<<std::endl;
    //    std::cout<<b<<std::endl;
    //Fill matrix c
    for(int i=0;i<s.size();i++){
        for (int j=1;j<=nrOfNodes;j++){
            if(s.at(i)->getNodep()==j){
                c(i,j-1)=1;
            }
            if(s.at(i)->getNodem()==j){
                c(i,j-1)=-1;
            }
        }
    }
    //    std::cout<<"matrix c"<<std::endl;
    //    std::cout<<c<<std::endl;
    //    std::cout<<"matrix d"<<std::endl;
    //    std::cout<<d<<std::endl;
    a.resize(g.rows()+c.rows(),b.cols()+g.cols());
    a<<g,b,
            c,d       ;

    //    std::cout<<"matrix a"<<std::endl;
    // std::cout<<a<<std::endl;


    //fill e matrix
    for(int i=0;i<s.size();i++){
        e(i,0)=s.at(i)->getValue();
    }


    z<<i,
            e;

    VectorXf x = a.colPivHouseholderQr().solve(z);

    std::vector<float> solu;
    solu.push_back(0);   //Add value of ground node, always 0
    for (int i=0;i<nrOfNodes-1;i++){
        solu.push_back(x(i));
    }


    return solu;




}



