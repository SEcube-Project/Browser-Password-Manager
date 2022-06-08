export interface LocalStorage {
  options?: LocalStorageOptions;
}

export interface LocalStorageOptions {
  is_autocomplete_enabled: boolean;
  lock_after_minutes: number;
  is_locked: boolean;
}

export type LocalStorageKeys = keyof LocalStorage;

export function setStoredOptions(options: LocalStorageOptions): Promise<void> {
  const values: LocalStorage = {
    options,
  };
  return new Promise((resolve, reject) => {
    chrome.storage.local.set(values, () => {
      if (chrome.runtime.lastError) {
        reject(chrome.runtime.lastError);
      } else {
        resolve();
      }
    });
  });
}

export function getStoredOptions(): Promise<LocalStorageOptions> {
  const keys: LocalStorageKeys[] = ["options"];
  return new Promise((resolve, reject) => {
    chrome.storage.local.get(keys, (res: LocalStorage) => {
      if (chrome.runtime.lastError) {
        reject(chrome.runtime.lastError);
      } else {
        resolve(res.options);
        // console.log(res.options);
      }
    });
  });
}



