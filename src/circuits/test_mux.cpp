#include <string>
#include <abycore/aby/abyparty.h>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        return 1;
    }
    e_role role;
    std::string arg_role{argv[1]};
    if (arg_role == "server")
    {
        role = SERVER;
    }
    else if (arg_role == "client")
    {
        role = CLIENT;
    }
    else
    {
        return 1;
    }

    char address[] = "127.0.0.1";

    auto party{std::make_unique<ABYParty>(
            role,
            address,
            get_sec_lvl(128), // seclvl
            32, // bitlength
            1, // threads
            MT_OT, // multiplication tripe generator
            4000000, // maxgates
            1337
    )};
    auto sharings{party->GetSharings()};
    auto circuit{static_cast<BooleanCircuit*>(sharings[S_YAO]->GetCircuitBuildRoutine())};

    share* s_in;
    if (role == CLIENT)
    {
        s_in = circuit->PutINGate(uint8_t{0}, 1, CLIENT);
    }
    else
    {
        s_in = circuit->PutDummyINGate(1);
    }

    auto s_zero{circuit->PutCONSGate(uint8_t{0}, 1)};
    auto s_one{circuit->PutCONSGate(uint8_t{1}, 1)};
    auto s_mux0{circuit->PutMUXGate(s_one, s_zero, s_in)};
    auto s_mux1{circuit->PutMUXGate(s_one, s_in, s_in)};
    auto s_print0{circuit->PutPrintValueGate(s_mux0, "sel = 0")};
    auto s_print1{circuit->PutPrintValueGate(s_mux1, "sel = 1")};

    party->ExecCircuit();

    delete s_in;
    delete s_zero;
    delete s_one;
    delete s_mux0;
    delete s_mux1;
    delete s_print0;
    delete s_print1;
}
