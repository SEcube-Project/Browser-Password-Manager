import { Box } from "@mui/material";
import React, { useEffect, useState } from "react";
import FixedBottomNavigation from "./components/main";

const App: React.FC<{}> = () => {

  const [pageHostname, setPageHostname] = useState("");
  
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

  return (
    <div>
      <Box sx={{maxWidth: "100%", maxHeight: "100%"}}>
        <FixedBottomNavigation hostname={pageHostname}/>
      </Box>
    </div>
  );
};


export default App;
