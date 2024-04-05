% Read the soil moisture channel data

% Store the channel ID for the moisture sensor channel.
channelID = 2454500;

% Provide the ThingSpeak alerts API key.  All alerts API keys start with TAK.
alertApiKey = 'TAK36v6o2ewFy+gl1Ys';

% Set the address for the HTTTP call
alertUrl="https://api.thingspeak.com/alerts/send";

% webwrite uses weboptions to add required headers.  Alerts needs a ThingSpeak-Alerts-API-Key header.
options = weboptions("HeaderFields", ["ThingSpeak-Alerts-API-Key", alertApiKey ]);

% Set the email subject.
alertSubject = sprintf("Soil Moisture Alert: Action Required on Farm Irrigation");

% Read the recent data.
moistureData = thingSpeakRead(channelID,'NumDays',30,'Field',3);

% Check to make sure the data was read correctly from the channel.
if isempty(moistureData)
    alertBody = ' No data read from plant. ';      
else
    % threshold value based on recent data.
    dryValue = 220;
    maxValue = 620;

    % Get the most recent point in the array of moisture data.
    lastValue = moistureData(end); 

    % Set the outgoing message
    if (lastValue<=dryValue)
        alertBody = 'We are writing to inform you that the sensor readings from the irrigation system have dropped below the critical threshold of 220. As per our irrigation management protocol, we kindly request you to stop irrigation immediately to avoid potential water wastage and crop damage. ';
    elseif (lastValue>maxValue)
        alertBody = 'We have observed that the sensor readings from the irrigation system have exceeded the threshold of 620, indicating the need to resume irrigation activities in your farm.In light of this, we kindly request you to initiate the irrigation process promptly to ensure proper hydration of your crops and optimal growth conditions ';
    end
end
 
 % Catch errors so the MATLAB code does not disable a TimeControl if it fails
try
    webwrite(alertUrl , "body", alertBody, "subject", alertSubject, options);
catch someException
    fprintf("Failed to send alert: %s\n", someException.message);
end
