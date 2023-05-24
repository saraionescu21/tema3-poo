#include "tema3-poo.h"

int main() {

    auto abonament = std::make_shared<Abonament>("Abonament de baza", 100, 30);
    auto abonament_premium = std::make_shared<Abonament_Premium>(10, "Abonament Premium", 150, 30);

    auto abonat1 = std::make_shared<Abonat>("0720123456", abonament, 1, "Popescu Ion", "1111111111111");
    auto abonat2 = std::make_shared<Abonat>("0730456789", abonament_premium, 2, "Ionescu Maria", "2222222222222");
    auto abonat3 = std::make_shared<Abonat>("0770456789", abonament_premium, 3, "Ionescu Sara", "2222222222333");

    auto abonat4 = std::make_shared<Abonat>();
    auto abonat5 = std::make_shared<Abonat>();

    connection_pool pool;
    Clienti& clienti = Clienti::getInstance();

    clienti.adauga_abonat(abonat1);
    clienti.adauga_abonat(abonat2);
    clienti.adauga_abonat(abonat3);

    int optiune = 0;
    int numar_abonati_premium;
    float suma_incasata;

    while (optiune != 5) {
        std::cout << "Meniu:\n";
        std::cout << "1. Adauga un abonat nou\n";
        std::cout << "2. Afiseaza abonatii existenti\n";
        std::cout << "3. Numarul de abonati premium\n";
        std::cout << "4. Suma totala incasata\n";
        std::cout << "5. Iesire din program\n";
        std::cout << "Alege o optiune: ";
        std::cin >> optiune;

        switch (optiune) {

        case 1: {
            int tip_abonament;
            std::cout << "Alege tipul de abonament:\n";
            std::cout << "1. Abonament standard\n";
            std::cout << "2. Abonament premium\n";
            std::cout << "Alege o optiune: ";
            std::cin >> tip_abonament;

            try {
                if (tip_abonament == 1) {
                    auto abonat = std::make_shared<Abonat>();
                    connection& c = pool.get_conn();

                    abonat->citire();
                    clienti.adauga_abonat(abonat);
                }
                else if (tip_abonament == 2) {
                    auto abonament_premium = std::make_shared<Abonament_Premium>();
                    connection& c = pool.get_conn();
                    abonament_premium->citire();
                    auto abonat = std::make_shared<Abonat>();
                    abonat->citire();
                    abonat->setabonament(abonament_premium);
                    clienti.adauga_abonat(abonat);
                }
                else {
                    std::cerr << "Optiune invalida\n";
                }
            }
            catch (const NumarTelefonInvalid<std::string>& e) {
                std::cerr << "Exceptie: " << e.what() << '\n';
            }
            catch (const CnpInvalid<std::string>& e) {
                std::cerr << "Exceptie: " << e.what() << '\n';
            }
            catch (std::runtime_error& err) { std::cout << err.what() << "\n"; }
            break;
        }
        case 2:
            clienti.afisare_abonati();
            break;

        case 3:
            numar_abonati_premium = clienti.numar_abonati_premium<Abonament_Premium>();
            std::cout << "Numar abonati premium: " << numar_abonati_premium << std::endl;
            break;

        case 4:
            suma_incasata = clienti.suma_totala_incasata<Abonament_Premium>();
            std::cout << "Suma incasata este: " << suma_incasata << std::endl;
            break;

        case 5:
            std::cout << "La revedere!\n";
            break;
        }
    }

    return 0;
}
