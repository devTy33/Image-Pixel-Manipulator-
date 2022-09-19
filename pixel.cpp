// Ty Anderson
//#38
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
// Make a Struct for the pixel color values of the vector.
struct Pixel
{
    int red;
    int green;
    int blue;
};

class Picture
{
    // Create a vector that stores a 1d vector of the pixels. "<Pixel>" because the struct contains each pixels color value.
    vector<Pixel> pix;

private:
    int width;
    int height;
    int maxIntensity;

public:
    Picture()
    {
        width = 0;
        height = 0;
        maxIntensity = 0;
    }
    // Prototype all the functions and make them public because they need to be accessed outside of the class.
public:
    const Pixel &get_pixel(int row, int col) const;

    Pixel &get_pixel(int row, int col);

    void set_pixel(int row, int col, const Pixel &px);

    void write_output(ostream &);

    void invert();

    void flip_x();

    void flip_y();

    bool read_input(istream &in);
};

int main(int argc, char **argv)
{
    // instanciate the object so I can call functions
    Picture obj1;
    ifstream fin;
    ofstream fout;
    string command;
    // argv[1] is the name of the input file. If it is a "-" we know that there is no input file so input must be read from cin.
    if (string(argv[1]) == "-")
    {
        obj1.read_input(cin);
    }
    // if there isn't a dash, open the file given in the command line argument with "argv[1]"
    else
    {
        fin.open(argv[1]);
        if (!fin.is_open())
        {
            return 0;
        }
        else
        {
            obj1.read_input(fin);
        }
    }
    // turn argv[3] into a string for symplicity 
    if (argc == 4)
    {
        command = argv[3];
    }
    // Check for both upper and lower case since the command line arguments are case sensitive 
    // call the respective functions based on what the user includes in the command line.
    if (command == "I" || command == "i")
    {
        obj1.invert();
    }
    else if (command == "Y" || command == "y")
    {
        obj1.flip_y();
    }
    else if (command == "X" || command == "x")
    {
        obj1.flip_x();
    }
    else
    {
    };
    // If there is no given output file than we should write it to cout.
    if (string(argv[2]) == "-")
    {
        obj1.write_output(cout);
    }
    else
    {   // if there is a given output send it to the file @argv[2]
        fout.open(argv[2]);
        if (!fout.is_open())
        {
            return 0;
        }

        else
        {
            obj1.write_output(fout);
        }
    }

    fin.close();
    fout.close();
}

void Picture::flip_x()
{   // make temp style variable to hold the top and bottom pixel values as we iterate through both loops.
    struct Pixel low;
    struct Pixel high;
    // divide the height by two because you are swapping over the x axis (swapping virtically (moving y values)).
    for (int i = 0; i < height / 2; i++)
    {
        for (int j = 0; j < width; j++)
        {
            high = get_pixel(i, j);
            // bottom cell will be in same column but different row
            low = get_pixel((height - 1 - i), j);
            // set high as low and low as high.
            set_pixel(i, j, low);
            set_pixel((height - 1 - i), j, high);
        }
    }
}
// same concept as flip x but we are swapping the roles of width and height, and we are using left and right instead of top and bottom.
void Picture::flip_y()
{   
    for (int i = 0; i < height; i++)
    {  
         // divide the width by 2 because you are swaping across the y axis (swapping horizontally (moving x values )).
        for (int j = 0; j < width / 2; j++)
        {
            Pixel right, left;
            left = get_pixel(i, j);
            right = get_pixel(i, width - 1 - j);
            set_pixel(i, j, right);
            set_pixel(i, width - 1 - j, left);
        }
    }
}

void Picture::write_output(ostream &out)
{
    // Print out in order of given examples: Filetype, width and height, max intesity, and finally pixel color values (3 for each)
    out << "P3"
        << "\n";
    out << width << " " << height << "\n";
    out << maxIntensity << "\n";
    // Loop through the pix vector and assign every pixel its color values. \n ensures no more than 3 color values per row when printed.
    for (int j = 0; j < width * height; j++)
    {   
        out << pix[j].red << " " << pix[j].green << " " << pix[j].blue << "\n";
    }
};

void Picture::invert()
{
    // Loop through the heigt and width so you can get the "coordinants" to pass to get_pixel.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Once you have the pixe from "get_pixel" invert the color values and set re-set them with "set_pixel"
            Pixel px = get_pixel(i, j);
            px.red = maxIntensity - px.red;
            px.green = maxIntensity - px.green;
            px.blue = maxIntensity - px.blue;
            set_pixel(i, j, px);
        }
    }
}
const Pixel &Picture::get_pixel(int row, int col) const
{
    int index;
    // this little equation will get the pixel number and we can use the vector to get the pixel values.
    index = (row * width) + col;
    return pix[index];
}

Pixel &Picture::get_pixel(int row, int col)
{
    int index;
    index = (row * width) + col;
    return pix[index];
}
// Used to swap positions in flip_x() and flip_y().
void Picture::set_pixel(int row, int col, const Pixel &px)
{
    // sends a row and a column to get pixel to get the pixel at its index.
    get_pixel(row, col) = px;
}

bool Picture::read_input(istream &in)
{
    string temp;
    stringstream a;
    string data;
    // instantiate the struct so we can access it
    Pixel px;
    // read in the file (in) and push it into the string (data)
    while (getline(in, data))
    {   //this ensures that the comments are ignored.
        if (data[0] == '#')
        {
            continue;
        }
        else
        {
            // if it's not a comment, add it to the string stream
            a << data << " ";
        }
    }
    a >> temp;
    // Since we are only reading in PPM files, we want to error check the file type.
    if (temp != "P3")
    {
        cout << "Improper file type" << endl;
        return false;
    }
    a >> width >> height;
    // By multiplying width and height, we can get the amount of pixels in the picture so we can reserve the space in our vector.
    pix.reserve(width * height);
    a >> maxIntensity;

    // Read in pixel colors and push them into vector
    while (a >> px.red >> px.green >> px.blue)
    {   // Check to make sure that all of the pixel's color values are less than the max intensity before they are pushed into the vector
        if (px.red > maxIntensity)
        {
            px.red = maxIntensity;
            cout << "larger than max intensity" << endl;
        }
        if (px.green > maxIntensity)
        {
            px.green = maxIntensity;
            cout << "larger than max intensity" << endl;
        }
        if (px.blue > maxIntensity)
        {
            px.blue = maxIntensity;
            cout << "larger than max intensity" << endl;
        }
        pix.push_back(px);
    }
    return true;
}



 
                                                                                                                                                                                          246,1         Bot
