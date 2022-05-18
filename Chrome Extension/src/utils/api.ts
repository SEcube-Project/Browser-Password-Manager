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

export interface device{
    index: number;
    path: string;
    serial: string;
}
export interface devices{
    count: number;
    devices: device[];
}

export interface generatedPassword{
    generated: string;
}

export interface removePassword{
    success: boolean;
}

/**
 * @param debug if true, it will use the password_debug.json file
 * @param type The type of request. Can be GET, POST, PUT, DELETE
 * @param url The url to send the request to
 */
export async function getAllPasswords(debug: boolean, requestType?:fetchType, url?: string): Promise<ApiBody> {
    if (!debug) {
        const res = await fetch(url, {
            method: requestType,
        });
        // check if the response is 200; if not throw an error
        if (!res.ok) {
            throw new Error(res.statusText);
        }
        // Declare data as a ApiBody type to take advantage of the type checking
        const data: ApiBody = await res.json();
        return data;
    }
}

export async function getAllPasswordsByHostname(debug: boolean, requestType?:fetchType, url?: string): Promise<ApiBody> {
    if (!debug) {
        const res = await fetch(url, {
            method: requestType,
        });
        // check if the response is 200; if not throw an error
        if (!res.ok) {
            throw new Error(res.statusText);
        }
        // Declare data as a ApiBody type to take advantage of the type checking
        const data: ApiBody = await res.json();
        return data;
    }
}

export async function insertNewPassword(debug: boolean, requestType?:fetchType, url?: string, body?: PasswordElement) {
    if (!debug) {
        const res = await fetch(url, {
            method: requestType,
            body: JSON.stringify(body),
        });
        // check if the response is 200; if not throw an error
        if (!res.ok) {
            throw new Error(res.statusText);
        }
    }
}

export async function updatePassword(debug: boolean, requestType?:fetchType, url?: string, body?: PasswordElement) {
    if (!debug) {
        const res = await fetch(url, {
            method: requestType,
            body: JSON.stringify(body),
        });
        // check if the response is 200; if not throw an error
        if (!res.ok) {
            throw new Error(res.statusText);
        }
    }
}

export async function getDevices(debug: boolean, requestType?:fetchType, url?: string): Promise<devices> {
    if (!debug) {
        const res = await fetch(url, {
            method: requestType,
        });
        // check if the response is 200; if not throw an error
        if (!res.ok) {
            throw new Error(res.statusText);
        }
        // Declare data as a devices type to take advantage of the type checking
        const data: devices = await res.json();
        return data;
    }
}

export async function deletePassword(debug: boolean, requestType?:fetchType, url?: string, body?: PasswordElement) {
    if (!debug) {
        const res = await fetch(url, {
            method: requestType,        
        });
        // check if the response is 200; if not throw an error
        if (!res.ok) {
            throw new Error(res.statusText);
        }
        const data: removePassword = await res.json();
        if (!data.success) {
            throw new Error("Success: false");
            
        }
    }
}


export async function generatePassword(debug: boolean, requestType?:fetchType, url?: string): Promise<generatedPassword> {
    if (!debug) {
        const res = await fetch(url, {
            method: requestType,
        });
        // check if the response is 200; if not throw an error
        if (!res.ok) {
            throw new Error(res.statusText);
        }
        // Declare data as a generatedPassword type to take advantage of the type checking
        const data: generatedPassword = await res.json();
        return data;
    }
}

