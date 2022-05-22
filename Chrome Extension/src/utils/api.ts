import password_debug from "../static/password_debug.json";

export type fetchType = "GET" | "POST" | "PUT" | "DELETE";

export interface PasswordElement {
  id: number;
  hostname: string;
  username: string;
  password: string;
}

export interface ApiBody {
  count: number;
  passwords: PasswordElement[];
}

export interface device {
  index: number;
  path: string;
  serial: string;
}
export interface devices {
  count: number;
  devices: device[];
}

export interface generatedPassword {
  generated: string;
}

export interface removePassword {
  success: boolean;
}

export async function getAllPasswords(
  pin: string,
  requestType?: fetchType
): Promise<ApiBody> {
  if (pin !== "") {
    const url = `https://127.0.0.1:5000/api/v0/device/0/passwords?pin=${pin}`;
    // const url = `https://127.0.0.1:5000/api/v0/device/0/passwords?pin=test`;
    const res = await fetch(url, {
      method: requestType,
    });
    // check if the response is 200; if not throw an error
    if (!res.ok) {
      throw new Error(res.statusText);
    }
    // Declare data as a ApiBody type to take advantage of the type checking
    const data: ApiBody = await res.json();
    console.log(data);
    return data;
  }
}

export async function getAllPasswordsByHostname(
  hostname: string,
  pin: string,
  requestType?: fetchType
): Promise<ApiBody> {
  const url = `https://127.0.0.1:5000/api/v0/device/0/passwords?pin=${pin}&hostname=${hostname}`;
  // const url = `https://127.0.0.1:5000/api/v0/device/0/passwords?pin=test&hostname=${hostname}`;

  if (hostname !== "" && pin !== "") {
    const res = await fetch(url, {
      method: requestType,
      headers: {
        "Access-Control-Allow-Origin": "*",
        "Access-Control-Allow-Headers": "*",
        "Access-Control-Allow-Methods": "*",
        "Content-Type": "application/json",
      },
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

export async function insertNewPassword(
  hostname: string,
  username: string,
  password: string,
  pin: string,
  requestType?: fetchType
) {
  if (hostname !== "" && username !== "" && password !== "" && pin != "") {
    const url = `https://127.0.0.1:5000/api/v0/device/0/passwords?pin=${pin}`;
    const body = JSON.stringify({
      hostname: hostname,
      username: username,
      password: password,
    });
    console.log(body);
    const res = await fetch(url, {
      method: requestType ? requestType : "POST",
      headers: {
        Accept: "application/json",
        "Content-Type": "application/json",
      },
      body: body,
    });
    // check if the response is 200; if not throw an error
    if (!res.ok) {
      throw new Error(res.statusText);
    }
  }
}

export async function updatePassword(
  id: number,
  hostname: string,
  username: string,
  password: string,
  pin: string,
  requestType?: fetchType
) {
  if (
    id !== null &&
    hostname !== "" &&
    username !== "" &&
    password !== "" &&
    pin !== ""
  ) {
    const url = `https://127.0.0.1:5000/api/v0/device/0/password/${id}?pin=${pin}`;
    if (
      id != null &&
      hostname != "" &&
      username != "" &&
      password != "" &&
      pin != ""
    ) {
      const res = await fetch(url, {
        method: requestType ? requestType : "PUT",
        headers: {
          Accept: "application/json",
          "Content-Type": "application/json",
        },
        body: JSON.stringify({
          id: id,
          hostname: hostname,
          username: username,
          password: password,
        }),
      });
      // check if the response is 200; if not throw an error
      if (!res.ok) {
        throw new Error(res.statusText);
      }
    }
  }
}

export async function getDevices(requestType?: fetchType): Promise<devices> {
  const url = `https://127.0.0.1:5000/api/v0/devices`;
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

export async function deletePassword(
  id: number,
  pin: string,
  requestType?: fetchType
) {
  if (id !== undefined && pin != "") {
    const url = `https://127.0.0.1:5000/api/v0/device/0/password/${id}?pin=${pin}`;
    const res = await fetch(url, {
      method: requestType ? requestType : "DELETE",
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

export async function generatePassword(
  upper: boolean,
  special: boolean,
  numbers: boolean,
  length: number,
  pin: string,
  requestType?: fetchType
): Promise<generatedPassword> {
  console.log(upper, numbers, special, length);
  if (
    pin !== "" &&
    upper !== undefined &&
    special !== undefined &&
    numbers !== undefined &&
    length !== undefined
  ) {
    const url = `https://127.0.0.1:5000/api/v0/device/0/generate?pin=${pin}&upper=${
      upper ? 1 : 0
    }&special=${special ? 1 : 0}&numbers=${numbers ? 1 : 0}&length=${length}`;
    console.log(url);

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

export async function login(pin: string): Promise<boolean> {
  const upper = true;
  const numbers = true;
  const special = true;
  const length = 5;
  if (pin !== "") {
    const url = `https://127.0.0.1:5000/api/v0/device/0/generate?pin=${pin}&upper=${
      upper ? 1 : 0
    }&special=${special ? 1 : 0}&numbers=${numbers ? 1 : 0}&length=${length}`;

    const res = await fetch(url);
    // check if the response is 200; if not throw an error
    if (!res.ok) {
      return false;
    } else {
      return true;
    }
  }
}
