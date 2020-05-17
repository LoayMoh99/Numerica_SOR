#ifndef VARIABLE_H
#define VARIABLE_H

struct variable {
    int row = 0;
    int column = 0;
    void operator++(int) {
        row++;
        return;
    }
};

#endif // VARIABLE_H
