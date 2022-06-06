import { Box } from "@mui/material";
import React, { useEffect, useState } from "react";
import { getStoredOptions } from "../utils/storage";
import FixedBottomNavigation from "./components/main";

const App: React.FC<{}> = () => {

  const [pageHostname, setPageHostname] = useState("");
  var value = 0;
  
  useEffect(() => {
    chrome.tabs.query({ active: true, lastFocusedWindow: true }, (tabs) => {
      var full_url = tabs[0].url;
      // Extract the hostname from the URL
      var page_hostname = full_url.split("/")[2];
      // remove www. from the string
      page_hostname = page_hostname.replace("www.", "");
      setPageHostname(page_hostname);
    });
  }, []);


  function checkIsLocked(): boolean {
    getStoredOptions().then((options) => {
      console.log("app options", options);
      if(options.is_locked){
        console.log("return: true")
        value = 4
        return true;
      } else {
        value = 0
        console.log("return: false")
        return false;
      }
    }
    );
    console.log("arrivato qua");
    return false;
  }



  return (
    <div>
      <Box sx={{maxWidth: "100%", maxHeight: "100%"}}>
        {checkIsLocked() && <FixedBottomNavigation hostname={pageHostname} default_state={4}/>}
        {!checkIsLocked() && <FixedBottomNavigation hostname={pageHostname} default_state={0}/>}
      </Box>
    </div>
  );
};


export default App;