#include <new>
#include <numeric>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <array>
#include "../Host Password Manager/sources/L1/L1.h"
#include "wrapper.hpp"

/**
 * @brief Create a new L1 object.
 * @return A pointer to the new L1 object.
 */
EXPORT_FUNC(void, *createL1Instance)() {
    return new(std::nothrow) L1;
}

/**
 * @brief Destroy a L1 object.
 * @param void *instance: A pointer to the L1 object to be destroyed.
 * @return void.
 */
EXPORT_FUNC(void, destroyL1Instance)(void *instance) {
    delete (L1 *)instance;
}

/**
 * @brief Select a specific SECube device by its index.
 * @param void *instance: A pointer to the L1 object.
 * @param char index: The index of the device to be selected.
 * @return void.
 */
EXPORT_FUNC(void, L1_SelectSECube_Indx)(void *instance, char indx) {
    L1 *l1 = reinterpret_cast<L1 *>(instance);
    l1->L1SelectSEcube(indx);
}

/**
 * @brief Logins to the selected SECube device.
 * @param void *instance: A pointer to the L1 object.
 * @param uint8_t pin[32]: The PIN of the device.
 * @param uint8_t is_admin: login as admin or not.
 * @param uint8_t do_force: force login or not.
 * @return int: 1 for success, 0 for failure.
 */
EXPORT_FUNC(int, L1_Login)(void *instance, uint8_t pin[32], uint8_t is_admin, uint8_t do_force) {

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

/**
 * @brief Logouts from the selected SECube device.
 * @param void *instance: A pointer to the L1 object.
 * @return int: 1 for success, 0 for failure.
 */
EXPORT_FUNC(int, L1_Logout)(void *instance) {

    L1 *l1 = reinterpret_cast<L1 *>(instance);
    l1->L1Logout();

    return !l1->L1GetSessionLoggedIn();
}

/**
 * @brief Get the maximum length for hostSize, userSize, and passwordSize and the total number of stored passwords.
 * @param void *instance: A pointer to the L1 object.
 * @param uint16_t *maxHostSize: A pointer to the buffer to store the maximum length of host.
 * @param uint16_t *maxUserSize: A pointer to the buffer to store the maximum length of user.
 * @param uint16_t *maxPasswordSize: A pointer to the buffer to store the maximum length of password.
 * @param uint16_t *totallen: A pointer to the buffer to store the total number of stored passwords.
 * @param char *hostfilter: A pointer to the buffer where the hostname to use as filter is stored.
 * @param uint16_t hostfilterlen: The length of the hostname to use as filter.
 * @return void.
 */
EXPORT_FUNC(void, L1_GetPasswords_Sizes)(void *instance, uint16_t *maxHostSize, uint16_t *maxUserSize, uint16_t *maxPassSize, uint16_t *totallen, char *hostfilter, uint16_t hostfilterlen) {

    L1 *l1 = reinterpret_cast<L1 *>(instance);

    if (maxHostSize) *maxHostSize = 0;
    if (maxUserSize) *maxUserSize = 0;
    if (maxPassSize) *maxPassSize = 0;
    
    std::vector<se3Pass> passwords;
    /* if hostfilter is not null, we use it as filter */
    if (hostfilter && hostfilterlen > 0) {
        l1->L1SEGetAllPasswordsByHostName(std::vector<uint8_t>(hostfilter, hostfilter + hostfilterlen), passwords);
    } else {
        l1->L1SEGetAllPasswords(passwords);
    }

    /* get the maximum length of host, user, and password */
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

/**
 * @brief Get the list of stored passwords.
 * @param void *instance: A pointer to the L1 object.
 * @param uint32_t *ids: A pointer to the buffer to store the IDs of the stored passwords.
 * @param uint16_t *hostSizes: A pointer to the buffer to store the lengths of the hostnames.
 * @param uint16_t *userSizes: A pointer to the buffer to store the lengths of the usernames.
 * @param uint16_t *passSizes: A pointer to the buffer to store the lengths of the passwords.
 * @param char *hosts: A pointer to the buffer to store the hostnames.
 * @param char *users: A pointer to the buffer to store the usernames.
 * @param char *passwords: A pointer to the buffer to store the passwords.
 * @param char *hostfilter: A pointer to the buffer where the hostname to use as filter is stored.
 * @param uint16_t hostfilterlen: The length of the hostname to use as filter.
 * @return int: 1 for success, 0 for failure.
 */
EXPORT_FUNC(int, L1_GetPasswords)(void *instance, uint32_t *ids, uint16_t *hostSizes, uint16_t *userSizes, uint16_t *passSizes, char *hosts, char *users, char *passwords, char *hostfilter, uint16_t hostfilterlen) {

    L1 *l1 = reinterpret_cast<L1 *>(instance);

    std::vector<se3Pass> pwds;

    bool res;
    if (hostfilter && hostfilterlen > 0) {
        res = l1->L1SEGetAllPasswordsByHostName(std::vector<uint8_t>(hostfilter, hostfilter + hostfilterlen), pwds);
    } else {
        res = l1->L1SEGetAllPasswords(pwds);
    }

    // std::transform takes pwds, and stores in the third argument each element of pwds but as result of the
    // lambda function passed as last argument.
    std::transform(pwds.begin(), pwds.end(), ids, [](se3Pass &p) { return p.id; });
    std::transform(pwds.begin(), pwds.end(), hostSizes, [](se3Pass &p) { return p.hostSize; });
    std::transform(pwds.begin(), pwds.end(), userSizes, [](se3Pass &p) { return p.userSize; });
    std::transform(pwds.begin(), pwds.end(), passSizes, [](se3Pass &p) { return p.passSize; });
    
    size_t hosts_itr = 0;
    size_t users_itr = 0;
    size_t pass_itr = 0;
    // concatenate the hostnames, usernames, and passwords into their buffers
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

/**
 * @brief Stores a new password.
 * @param void *instance: A pointer to the L1 object.
 * @param uint8_t *host_data: A pointer to the buffer containing the hostname.
 * @param uint16_t host_len: The length of the hostname.
 * @param uint8_t *user_data: A pointer to the buffer containing the username.
 * @param uint16_t user_len: The length of the username.
 * @param uint8_t *pass_data: A pointer to the buffer containing the password.
 * @param uint16_t pass_len: The length of the password.
 * @return int: 1 for success, 0 for failure.
 */
EXPORT_FUNC(int, L1_AddPassword)(void *instance, uint8_t *host_data, uint16_t host_len, uint8_t *user_data, uint16_t user_len, uint8_t *pass_data, uint16_t pass_len) {

    L1 *l1 = reinterpret_cast<L1 *>(instance);

    std::vector<se3Pass> pwds;
    l1->L1SEGetAllPasswords(pwds);
    std::sort(pwds.begin(), pwds.end(), [](const se3Pass &a, const se3Pass &b) { return a.id < b.id; });

    uint32_t previd = -1;
    uint32_t useid = -1;
    // looking for the next available ID
    for (auto &p : pwds) {
        // if there is a gap (i.e. distance between current ID and previous ID is greater than 1), then use it
        if (previd != -1 && p.id - previd > 1) {
            useid = previd + 1;
            break;
        }

        previd = p.id;
    }

    // trick to fix the previous for loop in case there are no passwords stored yet, starts from id=1
    uint32_t id = !pwds.size() ? 1 : useid == -1 ? pwds[pwds.size() - 1].id + 1 : useid;

    auto res = l1->L1SEAddPassword(id, host_data, host_len, user_data, user_len, pass_data, pass_len);
    return res ? 1 : 0;
}

/**
 * @brief Modifies a password.
 * @param void *instance: A pointer to the L1 object.
 * @param uint32_t id: The ID of the password to modify.
 * @param uint8_t *host_data: A pointer to the buffer containing the hostname.
 * @param uint16_t host_len: The length of the hostname.
 * @param uint8_t *user_data: A pointer to the buffer containing the username.
 * @param uint16_t user_len: The length of the username.
 * @param uint8_t *pass_data: A pointer to the buffer containing the password.
 * @param uint16_t pass_len: The length of the password.
 * @return int: 1 for success, 0 for failure.
 */
EXPORT_FUNC(int, L1_ModifyPassword)(void *instance, uint32_t id, uint8_t *host_data, uint16_t host_len, uint8_t *user_data, uint16_t user_len, uint8_t *pass_data, uint16_t pass_len) {

    L1 *l1 = reinterpret_cast<L1 *>(instance);

    se3Pass pwd = { id, host_len, user_len, pass_len, std::string(host_data, host_data + host_len), std::string(user_data, user_data + user_len), std::string(pass_data, pass_data + pass_len) };
    auto res = l1->L1SEModifyPassword(id, pwd);

    return res ? 1 : 0;
}

/**
 * @brief Get a password by ID.
 * @param void *instance: A pointer to the L1 object.
 * @param uint32_t id: The ID of the password to get.
 * @param uint16_t *hostSize: A pointer to the variable to store the hostname length.
 * @param uint16_t *userSize: A pointer to the variable to store the username length.
 * @param uint16_t *passSize: A pointer to the variable to store the password length.
 * @param char *host: A pointer to the buffer to store the hostname.
 * @param char *user: A pointer to the buffer to store the username.
 * @param char *pass: A pointer to the buffer to store the password.
 * @return int: 1 for success, 0 for failure.
 */
EXPORT_FUNC(int, L1_GetPasswordByID)(void *instance, uint32_t id, uint16_t *hostSize, uint16_t *userSize, uint16_t *passSize, char *host, char *user, char *pass) {

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

/**
 * @brief Delete a password by ID.
 * @param void *instance: A pointer to the L1 object.
 * @param uint32_t id: The ID of the password to delete.
 * @return int: 1 for success, 0 for failure.
 */
EXPORT_FUNC(int, L1_DeletePassword)(void *instance, uint32_t id) {

    L1 *l1 = reinterpret_cast<L1 *>(instance);

    auto res = l1->L1SEDeletePassword(id);
    // std::cout << "#CC: L1_DeletePassword: " << res << std::endl;
    return res ? 1 : 0;
}

/**
 * @brief Generate a new password.
 * @param void *instance: A pointer to the L1 object.
 * @param uint16_t pass_len: The length of the password.
 * @param uint8_t enable_upper_case: 1 to enable uppercase, 0 to disable.
 * @param uint8_t enable_special_chars: 1 to enable special characters, 0 to disable.
 * @param uint8_t enable_numbers_chars: 1 to enable numbers, 0 to disable.
 * @param char *generated: A pointer to the buffer to store the generated password.
 * @return int: 1 for success, 0 for failure.
 */
EXPORT_FUNC(int, L1_GeneratePassword)(void *instance, uint16_t pass_len, uint8_t enable_upper_case, uint8_t enable_special_chars, uint8_t enable_numbers_chars, char *generated) {

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
