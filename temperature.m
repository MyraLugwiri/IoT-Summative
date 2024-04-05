
% Store the channel ID for the moisture sensor channel.
channelID = 2454500;

% Provide the ThingSpeak alerts API key.  All alerts API keys start with TAK.
alertApiKey = 'TAK36v6o2ewFy+gl1Ys';

% Set the address for the HTTTP call
alertUrl="https://api.thingspeak.com/alerts/send";

% webwrite uses weboptions to add required headers.  Alerts needs a ThingSpeak-Alerts-API-Key header.
options = weboptions("HeaderFields", ["ThingSpeak-Alerts-API-Key", alertApiKey ]);

% Set the email subject.
alertSubject = sprintf(" Temperature Alert: Action Required on Farm");

% Read the recent data.
temperatureData = thingSpeakRead(channelID,'NumDays',30,'Field',1);

% Check to make sure the data was read correctly from the channel.
if isempty(temperatureData)
    alertBody = 'No data read from sensor';      
else
    % setting the 15 degrees threshold value.
    minValue = 15;

    % Get the most recent point in the array of moisture data.
    lastValue = temperatureData(end); 

    % Set the outgoing message
    if (lastValue<=minValue)
        alertBody = 'Your attention is required. The current temperature on your farm has dropped below 15°C, posing a risk to your crops and livestock. To prevent potential damage, please promptly close any open spaces allowing cold air in.Thank you for your immediate action.';
    elseif (lastValue> 26)
        alertBody = 'Your farms temperature has exceeded 26°C, posing potential risks to your crops and livestock. Please open all enclosed spaces to prevent heat buildup.';
    end
end
 
 % Catch errors so the MATLAB code does not disable a TimeControl if it fails
try
    webwrite(alertUrl , "body", alertBody, "subject", alertSubject, options);
catch someException
    fprintf("Failed to send alert: %s\n", someException.message);
end
