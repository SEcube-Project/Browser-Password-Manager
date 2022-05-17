#include <new>
#include <iterator>
#include <iostream>
#include <algorithm>
#include "../Host Password Manager/sources/L1/L1.h"

extern "C" void *createL1Instance() {
    return new(std::nothrow) L1;
}

extern "C" void L1_SelectSECube_Indx(void *instance, char indx) {
    L1 *l1 = reinterpret_cast<L1 *>(instance);
    l1->L1SelectSEcube(indx);
}

extern "C" int L1_Login(void *instance, uint8_t pin[32], uint8_t is_admin, uint8_t do_force) {

    L1 *l1 = reinterpret_cast<L1 *>(instance);

    std::array<uint8_t, L1Parameters::Size::PIN> ppin;
    std::copy(pin, pin + 32, ppin.begin());

    try {
        l1->L1Login(ppin, is_admin ? SE3_ACCESS_ADMIN : SE3_ACCESS_USER, do_force == 1);
    } catch (...) {
        return false;
    }

    return l1->L1GetSessionLoggedIn();
}

extern "C" int L1_Logout(void *instance) {

    L1 *l1 = reinterpret_cast<L1 *>(instance);
    l1->L1Logout();

    return !l1->L1GetSessionLoggedIn();
}

extern "C" void L1_GetPasswords_Sizes(void *instance, uint16_t *maxHostSize, uint16_t *maxUserSize, uint16_t *maxPassSize, uint16_t *totallen, char *hostfilter, uint16_t hostfilterlen) {

    L1 *l1 = reinterpret_cast<L1 *>(instance);

    if (maxHostSize) *maxHostSize = 0;
    if (maxUserSize) *maxUserSize = 0;
    if (maxPassSize) *maxPassSize = 0;
    
    std::vector<se3Pass> passwords;
    if (hostfilter && hostfilterlen > 0) {
        l1->L1SEGetAllPasswordsByHostName(std::vector<uint8_t>(hostfilter, hostfilter + hostfilterlen), passwords);
    } else {
        l1->L1SEGetAllPasswords(passwords);
    }

    for (auto &p : passwords) {
        if (maxHostSize && p.hostSize > *maxHostSize)
            *maxHostSize = p.hostSize;
        if (maxUserSize && p.userSize > *maxUserSize)
            *maxUserSize = p.userSize;
        if (maxPassSize && p.passSize > *maxPassSize)
            *maxPassSize = p.passSize;
    }

    if (totallen) *totallen = passwords.size();

    // if (maxHostSize) std::cout << "#CC: maxHostSize: " << *maxHostSize << std::endl;
    // if (maxUserSize) std::cout << "#CC: maxUserSize: " << *maxUserSize << std::endl;
    // if (maxPassSize) std::cout << "#CC: maxPassSize: " << *maxPassSize << std::endl;
    // if (totallen) std::cout << "#CC: totallen: " << *totallen << std::endl;

}

extern "C" int L1_GetPasswords(void *instance, uint32_t *ids, uint16_t *hostSizes, uint16_t *userSizes, uint16_t *passSizes, char *hosts, char *users, char *passwords, char *hostfilter, uint16_t hostfilterlen) {

    L1 *l1 = reinterpret_cast<L1 *>(instance);

    std::vector<se3Pass> pwds;

    bool res;
    if (hostfilter && hostfilterlen > 0) 
        res = l1->L1SEGetAllPasswordsByHostName(std::vector<uint8_t>(hostfilter, hostfilter + hostfilterlen), pwds);
    else 
        res = l1->L1SEGetAllPasswords(pwds);
    
    std::transform(pwds.begin(), pwds.end(), ids, [](se3Pass &p) { return p.id; });
    std::transform(pwds.begin(), pwds.end(), hostSizes, [](se3Pass &p) { return p.hostSize; });
    std::transform(pwds.begin(), pwds.end(), userSizes, [](se3Pass &p) { return p.userSize; });
    std::transform(pwds.begin(), pwds.end(), passSizes, [](se3Pass &p) { return p.passSize; });
    
    size_t hosts_itr = 0;
    size_t users_itr = 0;
    size_t pass_itr = 0;
    for (auto &p : pwds) {
        std::copy(p.host.begin(), p.host.end(), hosts + hosts_itr);
        hosts_itr += p.hostSize;

        std::copy(p.user.begin(), p.user.end(), users + users_itr);
        users_itr += p.userSize;

        std::copy(p.pass.begin(), p.pass.end(), passwords + pass_itr);
        pass_itr += p.passSize;
    }

    return res ? 1 : 0;
}

extern "C" int L1_AddPassword(void *instance, uint8_t *host_data, uint16_t host_len, uint8_t *user_data, uint16_t user_len, uint8_t *pass_data, uint16_t pass_len) {

    L1 *l1 = reinterpret_cast<L1 *>(instance);

    uint16_t len;
    L1_GetPasswords_Sizes(instance, nullptr, nullptr, nullptr, &len, nullptr, 0);

    auto res = l1->L1SEAddPassword(++len, host_data, host_len, user_data, user_len, pass_data, pass_len);
    std::cout << "#CC: L1_AddPassword: " << res << std::endl;
    return res ? 1 : 0;
}

extern "C" int L1_ModifyPassword(void *instance, uint32_t id, uint8_t *host_data, uint16_t host_len, uint8_t *user_data, uint16_t user_len, uint8_t *pass_data, uint16_t pass_len) {

    L1 *l1 = reinterpret_cast<L1 *>(instance);

    se3Pass pwd = { id, host_len, user_len, pass_len, std::string(host_data, host_data + host_len), std::string(user_data, user_data + user_len), std::string(pass_data, pass_data + pass_len) };
    auto res = l1->L1SEModifyPassword(id, pwd);

    return res ? 1 : 0;
}

extern "C" int L1_GetPasswordByID(void *instance, uint32_t id, uint16_t *hostSize, uint16_t *userSize, uint16_t *passSize, char *host, char *user, char *pass) {

    L1 *l1 = reinterpret_cast<L1 *>(instance);

    se3Pass pwd;
    auto res = l1->L1SEGetPasswordById(id, pwd);

    if (hostSize) *hostSize = pwd.hostSize;
    if (userSize) *userSize = pwd.userSize;
    if (passSize) *passSize = pwd.passSize;

    if (host) std::copy(pwd.host.begin(), pwd.host.end(), host);
    if (user) std::copy(pwd.user.begin(), pwd.user.end(), user);
    if (pass) std::copy(pwd.pass.begin(), pwd.pass.end(), pass);

    return res ? 1 : 0;
}

extern "C" int L1_DeletePassword(void *instance, uint32_t id) {

    L1 *l1 = reinterpret_cast<L1 *>(instance);

    auto res = l1->L1SEDeletePassword(id);
    std::cout << "#CC: L1_DeletePassword: " << res << std::endl;
    return res ? 1 : 0;
}

extern "C" int L1_GeneratePassword(void *instance, uint16_t pass_len, uint8_t enable_upper_case, uint8_t enable_special_chars, uint8_t enable_numbers_chars, char *generated) {

    L1 *l1 = reinterpret_cast<L1 *>(instance);

    // print all arguments
    // std::cout << "#CC: L1_GeneratePassword: " << std::endl;
    // std::cout << "#CC: pass_len: " << pass_len << std::endl;
    // std::cout << "#CC: enable_upper_case: " << (enable_upper_case == 1 ? 1 : 0) << "\n";
    // std::cout << "#CC: enable_special_chars: " << (enable_special_chars == 1 ? 1 : 0) << "\n";
    // std::cout << "#CC: enable_numbers_chars: " << (enable_numbers_chars == 1 ? 1 : 0) << "\n";

    std::shared_ptr<uint8_t[]> pass(new uint8_t[pass_len], std::default_delete<uint8_t[]>());
    auto res = l1->L1SEGenerateRandomPassword(pass_len, enable_upper_case, enable_special_chars, enable_numbers_chars, pass);
    std::copy(pass.get(), pass.get() + pass_len, generated);

    return res ? 1 : 0;
}
