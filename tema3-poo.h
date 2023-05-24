#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <exception>

//using namespace std;


class connection {
private:
    bool opened = false;
public:
    void open() { opened = true; }
    bool free() const { return !opened; }
    void close() { opened = false; }
    ~connection() { close(); }
};

class connection_pool {
private:
    static const int max_conns = 2;
    std::vector<connection> conns{ max_conns };
public:
    connection& get_conn() {
        for (auto& conn : conns)
            if (conn.free()) {
                conn.open();
                return conn;
            }
        throw std::runtime_error("too many open connections!");
    }
};

class Abonament {
public:
    Abonament() {}

    Abonament(std::string nume_abonament, float pret, int perioada)
        : nume_abonament(nume_abonament), pret(pret), perioada(perioada) {}

    Abonament(const Abonament& a) {
        nume_abonament = a.nume_abonament;
        pret = a.pret;
        perioada = a.perioada;
    }

    ~Abonament() {}

    Abonament& operator=(const Abonament& a) {
        nume_abonament = a.nume_abonament;
        pret = a.pret;
        perioada = a.perioada;
        return *this;
    }

    virtual void citire();
    virtual void afisare();

    void setnume_abonament(const std::string& other) {
        nume_abonament = other;
    }

    void setpret(const float& other) {
        pret = other;
    }

    void setperioada(const int& other) {
        perioada = other;
    }

    std::string get_nume_abonament() const {
        return nume_abonament;
    }

    float get_pret() const {
        return pret;
    }

    int get_perioada() const {
        return perioada;
    }

private:
    std::string nume_abonament;
    float pret;
    int perioada;
};

class Abonament_Premium : public Abonament {
public:
    Abonament_Premium() : Abonament() {
        reducere = 0;
    }

    Abonament_Premium(int reducere, std::string nume_abonament, float pret, int perioada)
        : reducere(reducere), Abonament(nume_abonament, pret, perioada) {}

    Abonament_Premium(const Abonament_Premium& a) : Abonament(a) {
        reducere = a.reducere;
    }

    Abonament_Premium& operator=(const Abonament_Premium& a) {
        Abonament::operator=(a);
        reducere = a.reducere;
        return *this;
    }

    ~Abonament_Premium() {}

    virtual void citire();
    virtual void afisare();

    void setreducere(const int& other) {
        reducere = other;
    }

    int get_reducere() const {
        return reducere;
    }

private:
    int reducere;
};

class Persoana {
public:
    Persoana() {}

    Persoana(int id, std::string nume, std::string cnp) : id(id), nume(nume), cnp(cnp) {}

    Persoana(const Persoana& p) {
        id = p.id;
        nume = p.nume;
        cnp = p.cnp;
    }

    Persoana& operator=(const Persoana& p) {
        id = p.id;
        nume = p.nume;
        cnp = p.cnp;
        return *this;
    }

    virtual void citire();
    virtual void afisare();

    void setid(const int& other) {
        id = other;
    }

    void setnume(const std::string& other) {
        nume = other;
    }

    void setcnp(const std::string& other) {
        cnp = other;
    }

    int get_id() const {
        return id;
    }
    std::string get_nume() const {
        return nume;
    }

    std::string get_cnp() const {
        return cnp;
    }

private:
    int id;
    std::string nume;
    std::string cnp;
};

template <typename T>
class NumarTelefonInvalid : public std::exception {
public:
    const char* what() const noexcept override {
        return "Numarul de telefon este invalid.";
    }
};

template <typename T>
class CnpInvalid : public std::exception {
public:
    const char* what() const noexcept override {
        return "CNP-ul este invalid.";
    }
};

class Abonat : public Persoana {
public:
    Abonat() : Persoana(), nr_telefon(""), x() {}

    Abonat(std::string nr_telefon, std::shared_ptr<Abonament> abonament, int id, std::string nume, std::string cnp) : nr_telefon(nr_telefon), abonament(abonament), Persoana(id, nume, cnp), x(*abonament) {}

    Abonat(const Abonat& a) : Persoana(a) {
        nr_telefon = a.nr_telefon;
        abonament = a.abonament;
        x = a.x;
    }

    Abonat& operator=(const Abonat& a) {
        Persoana::operator=(a);
        nr_telefon = a.nr_telefon;
        abonament = a.abonament;
        x = a.x;
        return *this;
    }

    virtual void citire() override {
        Persoana::citire();
        std::cout << "Numar de telefon: ";
        std::cin >> nr_telefon;

        if (nr_telefon.length() != 10) {
            throw  NumarTelefonInvalid<std::string>();
        }

        x.citire();

        if (get_cnp().length() != 13) {
            throw CnpInvalid<std::string>();
        }
    }


    virtual void afisare() override {
        Persoana::afisare();
        std::cout << "Numar de telefon: " << nr_telefon << "\n";
        std::cout << "Abonament: ";
        x.afisare();
    }

    void setnr_telefon(const std::string& other) {
        nr_telefon = other;
    }

    void setabonament(std::shared_ptr<Abonament> other) {
        abonament = other;
        x = *abonament;
    }

    std::string get_nr_telefon() const {
        return nr_telefon;
    }

    std::shared_ptr<Abonament> get_abonament() const {
        return abonament;
    }

private:
    std::string nr_telefon;
    std::shared_ptr<Abonament> abonament;
    Abonament x;
};

class Clienti {
public:
    Clienti() {}

    ~Clienti() {}

    static Clienti& getInstance() {
        static Clienti instance;
        return instance;
    }

    
    void adauga_abonat(std::shared_ptr<Abonat> abonat) {
        abonati.push_back(abonat);
    }

    void afisare_abonati() const {
        std::cout << "Lista de abonati:\n";
        for (const auto& abonat : abonati) {
            std::cout << "Nume: " << abonat->get_nume() << std::endl;
            std::cout << "CNP: " << abonat->get_cnp() << std::endl;
            std::cout << "Numar telefon: " << abonat->get_nr_telefon() << std::endl;
            std::cout << "Nume abonament: " << abonat->get_abonament()->get_nume_abonament() << std::endl;
            std::cout << "Pret: " << abonat->get_abonament()->get_pret() << std::endl;
            std::cout << "Perioada: " << abonat->get_abonament()->get_perioada() << std::endl;
            if (auto abonament_premium = dynamic_cast<Abonament_Premium*>(abonat->get_abonament().get())) {
                std::cout << "Reducere: " << abonament_premium->get_reducere() << std::endl;
            }
            std::cout << std::endl;
        }
    }

    template <typename T>
    int numar_abonati_premium() const {
        int count = 0;
        for (const auto& abonat : abonati) {
            if (dynamic_cast<T*>(abonat->get_abonament().get())) {
                count++;
            }
        }
        return count;
    }

    template <typename T>
    float suma_totala_incasata() const {
        float suma = 0;
        for (const auto& abonat : abonati) {
            int perioada = abonat->get_abonament()->get_perioada();
            float pret_luna = abonat->get_abonament()->get_pret();
            if (auto abonament_premium = dynamic_cast<T*>(abonat->get_abonament().get())) {
                float reducere = abonament_premium->get_reducere() / 100.0;
                pret_luna *= (1 - reducere);
            }
            suma += perioada * pret_luna;
        }
        return suma;
    }

private:
    std::vector<std::shared_ptr<Abonat>> abonati;
};


void Abonament::citire() {
    std::cout << "Nume_abonament: ";
    std::cin >> nume_abonament;
    std::cout << "Pret: ";
    std::cin >> pret;
    std::cout << "Perioada: ";
    std::cin >> perioada;
}

void Abonament::afisare() {
    std::cout << "Nume_abonament: " << nume_abonament;
    std::cout << " Pret: " << pret;
    std::cout << " Perioada: " << perioada << std::endl;
}

void Abonament_Premium::citire() {
    Abonament::citire();
    std::cout << "Reducere: ";
    std::cin >> reducere;
}

void Abonament_Premium::afisare() {
    Abonament::afisare();
    std::cout << "Reducere: " << reducere << std::endl;
}

void Persoana::citire() {
    std::cout << "ID: ";
    std::cin >> id;
    std::cout << "Nume: ";
    std::cin.ignore();
    std::getline(std::cin, nume);
    std::cout << "CNP: ";
    std::cin >> cnp;
}

void Persoana::afisare() {
    std::cout << "Id " << id;
    std::cout << " Nume " << nume;
    std::cout << " Cnp " << cnp << std::endl;
}