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

/**
 * @param debug if true, it will use the password_debug.json file
 * @param type The type of request. Can be GET, POST, PUT, DELETE
 * @param url The url to send the request to
 */
export async function getAllPasswords(
  requestType?: fetchType
): Promise<ApiBody> {
  const url = `https://127.0.0.1:5000/api/v0/device/0/passwords?pin=test`;
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

export async function getAllPasswordsByHostname(
  hostname: string,
  requestType?: fetchType
): Promise<ApiBody> {
  const url = `https://127.0.0.1:5000/api/v0/device/0/passwords?pin=test&hostname=${hostname}`;
  const res = await fetch(url, {
    method: requestType,
    headers: {
      "Access-Control-Allow-Origin": "*",
      "Access-Control-Allow-Headers": "*",
      "Access-Control-Allow-Methods": "*",
      "Content-Type": "application/json",
    }
  });
  // check if the response is 200; if not throw an error
  if (!res.ok) {
    throw new Error(res.statusText);
  }
  // Declare data as a ApiBody type to take advantage of the type checking
  const data: ApiBody = await res.json();
  return data;
}

export async function insertNewPassword(
  hostname: string,
  username: string,
  password: string,
  requestType?: fetchType
) {
  const url = `https://127.0.0.1:5000/api/v0/device/0/passwords?pin=test`;
  const body = JSON.stringify({
    hostname: hostname,
    username: username,
    password: password
  });
  console.log(body);
  const res = await fetch(url, {
    method: requestType ? requestType : "POST",
    headers: {
      'Accept': 'application/json',
      'Content-Type': 'application/json'
    },
    body: body
  });
  // check if the response is 200; if not throw an error
  if (!res.ok) {
    throw new Error(res.statusText);
  }
}

export async function updatePassword(
  id: number,
  hostname: string,
  username: string,
  password: string,
  requestType?: fetchType
) {
  const url = `https://127.0.0.1:5000/api/v0/device/0/password/${id}?pin=test`;
  const res = await fetch(url, {
    method: requestType ? requestType : "PUT",
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

export async function deletePassword(id: number, requestType?: fetchType) {
  const url = `https://127.0.0.1:5000/api/v0/device/0/password/${id}?pin=test`;
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

export async function generatePassword(
  upper: boolean,
  numbers: boolean,
  special: boolean,
  length: number,
  requestType?: fetchType
): Promise<generatedPassword> {
  console.log(upper, numbers, special, length);
  const url = `https://127.0.0.1:5000/api/v0/device/0/generate?pin=test&upper=${
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
