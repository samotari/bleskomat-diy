# Changelog

* v1.3.0-Bleskomat:
	* added PNG screen support. Derived from work of github nic @admzvk
* v1.3.0:
	* Bill acceptor support
	* Debounce hx616 pulse counting to prevent extraneous accumulated value. Don't count pulses caused by reconnecting coin signal pin, or when powering on the hx616, or when changing the physical switches on the hx616. Still possible but less likely as pulse high/low must be 20-200ms.
	* Fix QR code too small when text_size increased
* v1.2.5:
	* Improved reliability of counting pulses with hx616 coin acceptor
	* Fix for non-integer coin increment value
	* Disinhibit DG600F after initialization
* v1.2.4:
	* Better initialization state handling
* v1.2.3:
	* Fix getInt/Short/Float config value when set to empty-string
* v1.2.2:
	* Added spiffsFormatted config flag
	* Return spiffsInitialized flag in getinfo JSON-RPC command
	* Send response for restart JSON-RPC command
* v1.2.1:
	* Smaller log file size
* v1.2.0:
	* Use SPIFFS file system to store logs
	* JSON-RPC via serial - echo, getconfig, setconfig, getlogs, deletelogs, spiffs_reformat, restart
	* Move coin/button build flags to configuration options
	* Button delay no longer blocks the application loop
