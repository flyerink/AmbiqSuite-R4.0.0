09/01/2021

Once the installation is completed, IAR EWARM will support the Apollo4 Rev B
device with Secure Bootloader, AMAP42KK-KBR.

1. Start a Command Prompt as Administrator.
    - Press Windows+R - opens the "Run" box.
    - Type "cmd", Ctrl+Shift+Enter, click "Yes" to the system prompt.

2. In the Command Prompt, CD to the target IAR EWARM installation.  Typically
   this is at:
   C:\Program Files (x86)\IAR Systems\

3. Note the exact name of the target installation as 'a' or 'b' as follows:
   a) If IAR installed in a standard way on the C: drive (most installations),
      note the exact name of the installation. e.g. Embedded Workbench 8.5
      (Use no quotation marks for this method.)
   OR
   b) If IAR installed on other than the C: drive, note the full path of the
      IAR EWARM installation.  e.g. "D:\Program Files (x86)\IAR Systems\
      Embedded Workbench 8.5\arm\config\flashloader\AmbiqMicro"
      (Quotation marks must surround the full pathname.)

4. From the installation directory run
   install_Ambiq2IAR.bat <target name from step 3>  e.g.
    a) install_Ambiq2IAR.bat Embedded Workbench 8.5
       (Use no quotation marks for this method.)
    OR
    b) install_Ambiq2IAR.bat fullpath "D:\Program Files (x86)\IAR Systems\
      Embedded Workbench 8.5\arm\config\flashloader\AmbiqMicro"
      (Quotation marks must surround the full pathname.)

5. This should complete the update to IAR EWARM with Apollo4 support.
   Type exit to close the Command Prompt.
