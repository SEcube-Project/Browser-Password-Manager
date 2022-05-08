import {getStoredOptions, LocalStorageOptions, setStoredOptions } from "../utils/storage"

chrome.runtime.onInstalled.addListener(() => {
  const values: LocalStorageOptions = {
    autocomplete: true,
    lock_after_minutes: 5,
  };
  setStoredOptions(values);
  console.log(getStoredOptions());
})
