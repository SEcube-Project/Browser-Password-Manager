import password_debug from "../static/password_debug.json";

export type fetchType = "GET" | "POST" | "PUT" | "DELETE";


export interface PasswordElement  {
    id: number;
    hostname: string;
    username: string;
    password: string;
};

export interface ApiBody {
    count: number;
    passwords: PasswordElement[];
};


/**
 * This function is used to talk with gabriele socket
 * @param debug if true, it will use the password_debug.json file
 * @param type The type of request. Can be GET, POST, PUT, DELETE
 * @param url The url to send the request to
 */
export async function fetchApi(debug: boolean, requestType?:fetchType, url?: string): Promise<ApiBody> {
    if (!debug) {
        const res = await fetch(url, {
            method: requestType,
        });
        // check if the response is 200; if not throw an error
        if (!res.ok) {
            throw new Error(res.statusText);
        }
        // Declare data as a PasswordElements type to take advantage of the type checking
        const data: ApiBody = await res.json();
        return data;
    } else {
        // Read the file and pase it to the PasswordElements type
        const data: ApiBody = JSON.parse(JSON.stringify(password_debug));
        return data;
    }
}
