#include <iostream>
#include <fstream>

using namespace std;

void plot(const int x,const int y,const int z,int* matrix,fstream& ios)
{
    ios << "set terminal pngcairo  transparent enhanced font \"arial,10\" fontscale 1.0 size 3200, 3200" << '\n'  
        << "set output 'plot.png'" << '\n'
        << "unset key" << '\n'
        << "set style increment default" << '\n'
        << "set view map scale 1" << '\n'
        << "set style data lines" << '\n'
        << "set xtics border in scale 0,0 mirror norotate  autojustify" << '\n'
        << "set ytics border in scale 0,0 mirror norotate  autojustify" << '\n'
        << "set ztics border in scale 0,0 nomirror norotate  autojustify" << '\n'
        << "unset cbtics" << '\n'
        << "set rtics axis in scale 0,0 nomirror norotate  autojustify" << '\n'
        << "set title \"Routing Plot\"" << '\n'
        << "set xrange [ -0.500000 : " << x - 0.5 << "0000 ] noreverse nowriteback" << '\n'
        << "set x2range [ * : * ] noreverse writeback" << '\n'
        << "set yrange [ -0.500000 : " << y - 0.5 << "0000 ] noreverse nowriteback" << '\n'
        << "set y2range [ * : * ] noreverse writeback" << '\n'
        << "set zrange [ * : * ] noreverse writeback" << '\n'
        << "set cbrange [ 0.00000 : 4.00000 ] noreverse nowriteback" << '\n'
        << "set rrange [ * : * ] noreverse writeback" << '\n'
        << "set palette rgbformulae 2, -7, -7" << '\n'
        << "## Last datafile plotted: \"$map1\"" << '\n'
        << "$map1 << EOD" << '\n';
        int m = 0;
        for(int i=0; i<z; i++)
        {
           for(int j=0; j<y; j++)
           {
               for(int k=0; k<x; k++)
               {
                   ios << matrix[m] << ' ';
                   m++;
               }
               ios << '\n';
           }
        }
    ios << "EOD" << '\n'
        << "splot '$map1' matrix with image"; 
}

int main(int argc, char* argv[])
{
    // strart
    if (argc != 3) { cout << "Usage: ./data <input file name> <output file name>" << endl; return 0; }
    
    // open files
    fstream infile(argv[1]);
    if(!infile.is_open()) { cout << "No such file or directory: " << argv[1] << endl; return 0; }
    fstream outfile;
    outfile.open(argv[2]);
    if(!outfile.is_open()) { cout << "No such file or directory: " << argv[2] << endl; return 0; }

    // deal with infile
    int x,y,z;
    infile >> x >> y >> z;
    int area = x*y;
    int matrix[x*y*z] = {0};
    unsigned x1,y1,z1,x2,y2,z2;
    while(infile >> x1 >> y1 >> z1 >> x2 >> y2 >> z2)
    {
        matrix[z1*area+y1*x+x1] = 4;
        matrix[z2*area+y2*x+x2] = 4;
    }
    infile.close();

    // deal with outfile
    while(outfile >> x1 >> y1 >> z1)
    {
        if(matrix[z1*area+y1*x+x1] != 4) matrix[z1*area+y1*x+x1] = 2;
    }
    outfile.close();

    //output
    fstream outputfile;
    outputfile.open("plot.gp",ios::out);
    plot(x,y,z,matrix,outputfile);
    outputfile.close();

    return 0;

}