import { getStoredOptions, setStoredOptions } from "../storage.ts/storage"

chrome.runtime.onInstalled.addListener(() => {
  getStoredOptions();
  setStoredOptions({
    autocomplete: true,
    lock_after_minutes: 5,
  });
})
