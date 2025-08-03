//
// Created by werwolf2303 on 8/3/25.
//

#ifndef CORE_H
#define CORE_H
#include <QDialog>


class core {
  private:
    int coreCount;

  public:
    core();
    int getCoreCount() { return coreCount; }
    int getCoreState(int core);
    void setCoreState(int core, int state, QDialog *parent);
};



#endif //CORE_H
