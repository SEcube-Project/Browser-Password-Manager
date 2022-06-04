import {
  getStoredOptions,
  LocalStorageOptions,
  setStoredOptions,
} from "../utils/storage";

chrome.runtime.onInstalled.addListener(() => {
  const values: LocalStorageOptions = {
    is_autocomplete_enabled: true,
    lock_after_minutes: 5,
    is_locked: true,
    end_lock_time: 0,
  };
  setStoredOptions(values);
  // console.log(getStoredOptions());
  chrome.alarms.create("getNtpTime", {
    periodInMinutes: 1});
});

chrome.tabs.onUpdated.addListener((tabId, changeInfo, tab) => {
  // console.log(getStoredOptions());
  getStoredOptions().then((options) => {
    if (
      changeInfo.status === "complete" &&
      options.is_autocomplete_enabled === false &&
      options.is_locked === false
    ) {
      chrome.scripting.executeScript({
        target: { tabId: tabId },
        files: ["content.js"],
      });
    }
  });
});
