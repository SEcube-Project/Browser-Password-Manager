import {
  getStoredOptions,
  LocalStorageOptions,
  setStoredOptions,
} from "./storage";

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

export interface returnTime_string {
  time: string;
}

export interface returnTime_number {
  time: number;
}

export async function getAllPasswords(
  requestType?: fetchType
): Promise<ApiBody> {
  const url = `https://127.0.0.1:5000/api/v0/device/0/passwords`;
  const res = await fetch(url, {
    method: requestType,
    credentials: "include",
  });
  // check if the response is 200; if not throw an error
  setIsLockedValue(res);
  if (!res.ok) {
    throw new Error(res.statusText);
  }
  // Declare data as a ApiBody type to take advantage of the type checking
  const data: ApiBody = await res.json();
  // console.log("password from api", data);
  return data;
}

export async function getAllPasswordsByHostname(
  hostname: string,
  requestType?: fetchType
): Promise<ApiBody> {
  const url = `https://127.0.0.1:5000/api/v0/device/0/passwords?hostname=${hostname}`;

  if (hostname !== "") {
    const res = await fetch(url, {
      method: requestType,
      headers: {
        "Access-Control-Allow-Origin": "*",
        "Access-Control-Allow-Headers": "*",
        "Access-Control-Allow-Methods": "*",
        "Access-Control-Allow-Credentials": "true",
        "Content-Type": "application/json",
      },
      credentials: "include",
    });
    // check if the response is 200; if not throw an error
    setIsLockedValue(res);
    if (!res.ok) {
      throw new Error(res.statusText);
    }
    // Declare data as a ApiBody type to take advantage of the type checking
    const data: ApiBody = await res.json();
    // console.log("password from api hostname", data);
    return data;
  }
}

export async function insertNewPassword(
  hostname: string,
  username: string,
  password: string,
  requestType?: fetchType
) {
  if (hostname !== "" && username !== "" && password !== "") {
    const url = `https://127.0.0.1:5000/api/v0/device/0/passwords`;
    const body = JSON.stringify({
      hostname: hostname,
      username: username,
      password: password,
    });
    // console.log(body);
    const res = await fetch(url, {
      method: requestType ? requestType : "POST",
      headers: {
        Accept: "application/json",
        "Content-Type": "application/json",
      },
      credentials: "include",
      body: body,
    });
    // check if the response is 200; if not throw an error
    setIsLockedValue(res);
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
  requestType?: fetchType
) {
  if (id !== null && hostname !== "" && username !== "" && password !== "") {
    const url = `https://127.0.0.1:5000/api/v0/device/0/password/${id}`;
    if (id != null && hostname != "" && username != "" && password != "") {
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
        credentials: "include",
      });
      // check if the response is 200; if not throw an error
      setIsLockedValue(res);
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
    credentials: "include",
  });
  // check if the response is 200; if not throw an error
  setIsLockedValue(res);
  if (!res.ok) {
    throw new Error(res.statusText);
  }
  // Declare data as a devices type to take advantage of the type checking
  const data: devices = await res.json();
  return data;
}

export async function deletePassword(id: number, requestType?: fetchType) {
  if (id !== undefined) {
    const url = `https://127.0.0.1:5000/api/v0/device/0/password/${id}`;
    const res = await fetch(url, {
      method: requestType ? requestType : "DELETE",
      credentials: "include",
    });
    // check if the response is 200; if not throw an error
    setIsLockedValue(res);
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
  requestType?: fetchType
): Promise<generatedPassword> {
  // console.log(upper, numbers, special, length);
  if (
    upper !== undefined &&
    special !== undefined &&
    numbers !== undefined &&
    length !== undefined
  ) {
    const url = `https://127.0.0.1:5000/api/v0/device/0/generate?upper=${
      upper ? 1 : 0
    }&special=${special ? 1 : 0}&numbers=${numbers ? 1 : 0}&length=${length}`;
    // console.log(url);

    const res = await fetch(url, {
      method: requestType,
      credentials: "include",
    });
    // check if the response is 200; if not throw an error
    setIsLockedValue(res);
    if (!res.ok) {
      throw new Error(res.statusText);
    }
    // Declare data as a generatedPassword type to take advantage of the type checking
    const data: generatedPassword = await res.json();
    return data;
  }
}

export async function login(pin: string, timestamp: number): Promise<boolean> {
  console.log("pin", pin);
  if (pin !== "") {
      console.log("timestamp", timestamp);
      const url = `https://127.0.0.1:5000/api/v0/device/0/sessions?pin=${pin}&endtime=${timestamp}`;
      console.log(url);
      const res = await fetch(url, {
        method: "POST",
        credentials: "include",
      });
      setIsLockedValue(res);
      if (!res.ok) {
        console.log("failed");
        return false;
      } else {
        console.log("success");
        return true;
      }
  }
}


export async function getTime(): Promise<returnTime_number> {
  const url_time = `https://127.0.0.1:5000/api/v0/time`;
  const res_time = await fetch(url_time, {
    method: "GET",
    credentials: "include",
  });
  // check if the response is 200; if not throw an error
  if (!res_time.ok) {
    throw new Error(res_time.statusText);
  }
  const time_string: returnTime_string = await res_time.json();
  const time_number: returnTime_number = {
    time: parseInt(time_string.time),
  };

  return time_number;
}

export async function logout(): Promise<boolean> {
  const url = `https://127.0.0.1:5000/api/v0/device/0/sessions`;
  const res = await fetch(url, {
    method: "DELETE",
    credentials: "include",
  });
  // check if the response is 200; if not throw an error
  setIsLockedValue(res);
  if (!res.ok) {
    return false;
  } else {
    return true;
  }
}

export function setIsLockedValue(res: Response) {
  if (res.status === 403) {
    getStoredOptions().then((options) => {
      setStoredOptions({ ...options, is_locked: true });
      console.log(options);
    });
  }
}
