#include <QApplication>
#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QMap>
#include <QString>

// Estructura para almacenar la información de cada país
struct InfoPais {
    QString capital;
    QString idioma;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 1. Configuración de la Ventana Principal
    QWidget ventana;
    ventana.setWindowTitle("Información de Países - Qt Signals y Slots");
    ventana.resize(400, 300);

    // 2. Layout principal para organizar los elementos verticalmente
    QVBoxLayout *layoutPrincipal = new QVBoxLayout(&ventana);

    // Título de la sección
    QLabel *lblInstruccion = new QLabel("<b>Seleccione un país de la lista:</b>", &ventana);
    layoutPrincipal->addWidget(lblInstruccion);

    // 3. Creación de la lista de países (QListWidget)
    QListWidget *listaPaises = new QListWidget(&ventana);
    layoutPrincipal->addWidget(listaPaises);

    // Base de datos simulada (Diccionario de Países -> Capital e Idioma)
    QMap<QString, InfoPais> datosPaises = {
        {"Perú",    {"Lima", "Español"}},
        {"México",  {"Ciudad de México", "Español"}},
        {"España",  {"Madrid", "Español"}},
        {"Francia", {"París", "Francés"}},
        {"Japón",   {"Tokio", "Japonés"}},
        {"Canadá",  {"Ottawa", "Inglés / Francés"}},
        {"Italia",  {"Roma", "Italiano"}}
    };

    // Poblar la lista gráfica con las claves del diccionario
    for (auto it = datosPaises.cbegin(); it != datosPaises.cend(); ++it) {
        listaPaises->addItem(it.key());
    }

    // 4. Creación de las Etiquetas (Labels) para mostrar la información
    QLabel *lblCapital = new QLabel("<b>Capital:</b> -", &ventana);
    QLabel *lblIdioma = new QLabel("<b>Idioma:</b> -", &ventana);

    layoutPrincipal->addWidget(lblCapital);
    layoutPrincipal->addWidget(lblIdioma);

    // 5. Implementación de la Señal y el Slot
    // Conectamos la señal 'itemClicked' del widget de la lista con una función lambda (Slot)
    QObject::connect(listaPaises, &QListWidget::itemClicked, [lblCapital, lblIdioma, datosPaises](QListWidgetItem *item) {
        if (item) {
            QString paisSeleccionado = item->text();
            
            // Verificamos si existe en nuestra base de datos y actualizamos los Labels
            if (datosPaises.contains(paisSeleccionado)) {
                InfoPais info = datosPaises[paisSeleccionado];
                lblCapital->setText(QString("<b>Capital:</b> %1").arg(info.capital));
                lblIdioma->setText(QString("<b>Idioma:</b> %1").arg(info.idioma));
            }
        }
    });

    // Mostrar la ventana en pantalla
    ventana.show();

    // Ejecutar el bucle principal de la aplicación
    return app.exec();
}
