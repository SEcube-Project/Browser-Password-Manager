export interface LocalStorage {
  options?: LocalStorageOptions;
}

export interface LocalStorageOptions {
  autocomplete?: boolean;
  lock_after_minutes?: number;
}

export type LocalStorageKeys = keyof LocalStorage;

export function setStoredOptions(options: LocalStorageOptions): Promise<void> {
  const values: LocalStorage = {
    options,
  };
  return new Promise((resolve, reject) => {
    chrome.storage.local.set(options, () => {
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
      }
    });
  });
}
