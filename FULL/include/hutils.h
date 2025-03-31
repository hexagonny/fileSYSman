#ifndef HUTILS_H
#define HUTILS_H

#ifdef _WIN32
    #include <windows.h>
#else
    #include <cstdlib>
    #include <sys/ioctl.h>
    #include <unistd.h>
#endif

#include <string>
#include <sstream>
#include <regex>
#include <iomanip>

#ifndef HUTIL_API
#define HUTIL_API
#endif  

constexpr int SCREEN_WIDTH  = 50;
constexpr int SCREEN_HEIGHT = 30;

namespace hUtils {

    // --- SYSTEM UTILITIES ---
    HUTIL_API void setConsoleWindowSize();          //  Adjust console size.
    HUTIL_API void pause(bool clearBuffer = false); //  Cross-platform system pause.
    HUTIL_API void sleep(int milliseconds);         //  Sleep for a given duration.

    // --- TEXT UTILITIES ---
    struct Text {
    public:
        HUTIL_API void toLine            (char character = '-'); //  Print a line of repeated characters
        HUTIL_API void toCentered        (std::string text,      //  Prints centered text.
                                          int colorCode = 0,
                                          int number = 0,
                                          bool use256 = false);      
        HUTIL_API void toRight           (std::string text,      //  Moves text to the right.
                                          int colorCode = 0,
                                          bool use256 = false);  
        HUTIL_API void toLeft            (std::string text,      //  Moves text to the left.
                                          int colorCode = 0,
                                          int number = 0,
                                          bool use256 = false);
        HUTIL_API std::string toLowerCase(std::string text);     //  Convert string to lowercase.
        template <typename T>
        HUTIL_API std::string toString(const T& value,           //  Converts int and doubles to string.
                                       int precision)
        {
            std::ostringstream oss;
            if(precision >= 0 && std::is_floating_point<T>::value) {
                oss << std::fixed << std::setprecision(precision);
            }
            oss << value;
            return oss.str();
        }

        HUTIL_API std::string color      (int textColor = 0,     //  Get ANSI color codes.
                                          bool use256 = false);
        HUTIL_API std::string defaultText();                     //  Reset text color.

        HUTIL_API void clearAll          ();                     //  Clears every output in the terminal.
        HUTIL_API void clearBelow        (int line);             //  Clears an assigned line below it.
        HUTIL_API void clearAbove        (int line);             //  Clears an assigned line above it.
    };

    struct Table {
    private:
        std::vector<std::string> elements;
    
        std::string stripAnsi(const std::string& text) const
        {
            return std::regex_replace(text, std::regex("\033\\[[0-9;]*m"), "");
        }

        template <typename T>
        std::string toString (const T& value)
        {
            std::ostringstream oss;
            oss << value;
            return oss.str();
        }
        int calculateMaxWidth() const;
    
    public:
        template <typename... Args>
        HUTIL_API void setElements(Args... args)
        {
            elements.clear();
            (elements.push_back(toString(args)), ...);
        }
        HUTIL_API void toColumn   (std::string orientation = "left",
                                   int givenWidth = 0,
                                   int numberOfColumns = 2);
    };
    
    struct Bar {
    private:
        void printBar(double value,
                      double maxPoints,
                      int filledColor = 255,
                      int emptyColor  = 237);
    public:
        HUTIL_API void setBar  (double value,
                                double maxPoints,
                                int filledColor = 252,
                                int emptyColor  = 237);
    };
    
    // --- LOGGER UTILITIES ---
    struct Logger {
    private:
        int filesMoved = 0, foldersRemoved = 0, foldersCreated = 0, errors = 0, warnings = 0;
    
    public:
        HUTIL_API void Action     (const std::string& message);
        HUTIL_API void Success    (const std::string& message);
        HUTIL_API void Moved      (const std::string& message); //  filesMoved
        HUTIL_API void Removed    (const std::string& message); //  filesRemoved
        HUTIL_API void Created    (const std::string& message); //  foldersCreated
        HUTIL_API void Error      (const std::string& message); //  errors
        HUTIL_API void Warning    (const std::string& message); //  warnings
        HUTIL_API void Summary();
    };  
    extern Text   text;
    extern Table  table;
    extern Bar    bar;
    extern Logger log;      
}

#endif