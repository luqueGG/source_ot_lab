#include <iostream>
#include <memory>
#include <string>
using namespace std;

class Button {
public:
    virtual ~Button() {}
    virtual void Draw() const = 0;
};

class CheckBox {
public:
    virtual ~CheckBox() {}
    virtual void Draw() const = 0;
};

class WinButton : public Button {
public:
    void Draw() const override { cout << "Dibujando Button Windows" << endl; }
};

class MacButton : public Button {
public:
    void Draw() const override { cout << "Dibujando Button Mac" << endl; }
};

class LinuxButton : public Button {
public:
    void Draw() const override { cout << "Dibujando Button Linux" << endl; }
};

class WinCheckBox : public CheckBox {
public:
    void Draw() const override { cout << "Dibujando CheckBox Windows" << endl; }
};

class MacCheckBox : public CheckBox {
public:
    void Draw() const override { cout << "Dibujando CheckBox Mac" << endl; }
};

class LinuxCheckBox : public CheckBox {
public:
    void Draw() const override { cout << "Dibujando CheckBox Linux" << endl; }
};

class GUIFactory {
public:
    virtual ~GUIFactory() {}
    virtual unique_ptr<Button> CrearControlW() const = 0;
    virtual unique_ptr<CheckBox> CrearControlM() const = 0;
    virtual void Draw() const = 0;
};

class WinFactory : public GUIFactory {
public:
    unique_ptr<Button> CrearControlW() const override { return unique_ptr<Button>(new WinButton()); }
    unique_ptr<CheckBox> CrearControlM() const override { return unique_ptr<CheckBox>(new WinCheckBox()); }
    void Draw() const override {
        CrearControlW()->Draw();
        CrearControlM()->Draw();
    }
};

class MacFactory : public GUIFactory {
public:
    unique_ptr<Button> CrearControlW() const override { return unique_ptr<Button>(new MacButton()); }
    unique_ptr<CheckBox> CrearControlM() const override { return unique_ptr<CheckBox>(new MacCheckBox()); }
    void Draw() const override {
        CrearControlW()->Draw();
        CrearControlM()->Draw();
    }
    void draw(const WinFactory& colaborador) const {
        colaborador.Draw();
    }
};

class LinuxFactory : public GUIFactory {
public:
    unique_ptr<Button> CrearControlW() const override { return unique_ptr<Button>(new LinuxButton()); }
    unique_ptr<CheckBox> CrearControlM() const override { return unique_ptr<CheckBox>(new LinuxCheckBox()); }
    void Draw() const override {
        CrearControlW()->Draw();
        CrearControlM()->Draw();
    }
};

class Application {
public:
    Application(const GUIFactory& f, int os) {
        if (os < 1 || os > 3) {
            cout << "Sistema no soportado" << endl;
            return;
        }
        unique_ptr<Button> b = f.CrearControlW();
        b->Draw();
    }
};

int main() {
    cout << "Cliente : Windows" << endl;
    WinFactory* f1 = new WinFactory();
    Application(*f1, 1);
    delete f1;
    cout << endl;

    cout << "Cliente: Mac" << endl;
    MacFactory* f2 = new MacFactory();
    Application(*f2, 2);
    delete f2;
    cout << endl;

    cout << "Cliente: Linux" << endl;
    LinuxFactory* f3 = new LinuxFactory();
    Application(*f3, 3);
    f3->CrearControlM()->Draw();
    delete f3;
    return 0;
}