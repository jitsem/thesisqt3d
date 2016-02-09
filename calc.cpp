#include "calc.h"
#include <iostream>
#include <vector>
#include <QTextStream>
#include "Eigen/Dense"


//comment iets

using namespace Eigen;
Calc::Calc()
{

}

std::vector<float> Calc::solveLevel(QFile *file)
{
    std::vector<std::shared_ptr<Component>> sources;
    std::vector<std::shared_ptr<Component>> resistors;
    std::list<int> nodes;

    std::vector<std::shared_ptr<Component>> com = readFile(file);

    for(std::shared_ptr<Component> c:com){
        std::string s=typeid(*c).name();
       if(s.substr(1,s.size()-1) =="Source"){
           sources.push_back(c);
           nodes.push_back(c->getNodem());
           nodes.push_back(c->getNodep());
       }
       if(s.substr(1,s.size()-1)=="Resistor"){
           resistors.push_back(c);
           nodes.push_back(c->getNode1());
           nodes.push_back(c->getNode2());
       }

    }
    nodes.sort();
    nodes.unique();


    std::vector<float> sol=computeNetwork(sources,resistors,nodes.size());
    return sol;
}


std::vector<std::shared_ptr<Component>> Calc::readFile(QFile *file)
{
    std::vector<std::shared_ptr<Component>> components;
    if (file->open(QIODevice::ReadOnly| QIODevice::Text))
    {
        QTextStream in(file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList lineList = line.split(" ");

            switch ( lineList.at(0).toLower().at(0).toLatin1() )
            {
            case 'v':
            {
                auto v = std::make_shared<Source>(lineList.at(1).toFloat(),lineList.at(2).toInt(), lineList.at(3).toInt());
                components.push_back(v);
                break;
            }
            case 'r':
            {

                auto r = std::make_shared<Resistor>(lineList.at(1).toFloat(),lineList.at(2).toInt(), lineList.at(3).toInt());
                components.push_back(r);
                break;
            }
            default:
                std::cout<<"Fault in Reading File"<<std::endl;
            }

        }
        file->close();
    }
    return components;
}


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

    std::vector<float> sol;
    for (int i=0;i<nrOfNodes;i++){
        sol.push_back(x(i));
    }

    return sol;


}



