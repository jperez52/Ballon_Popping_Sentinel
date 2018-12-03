clear all
close all
clc
 objects = imaqfind;
 delete(objects);
vid = videoinput('winvideo',1,'YUY2_640x480');

% Set the properties of the video object
set(vid, 'FramesPerTrigger', Inf);
set(vid, 'ReturnedColorspace', 'rgb');
vid.FrameGrabInterval = 1;
 s=serial('COM3','BaudRate',115200);
 fopen(s);
%start the video aquisition here
start(vid)

% Set a loop
while(vid.FramesAcquired<=Inf)
    
    % Get the snapshot of the current frame
    data = getsnapshot(vid);
    
   
    diff_im = imsubtract(data(:,:,1), rgb2gray(data));
    %Use a median filter to filter out noise
    diff_im = medfilt2(diff_im, [3 3]);
    % Convert the resulting grayscale image into a binary image.
    diff_im = im2bw(diff_im,0.18);
    
    % Remove all those pixels less than 300px
    diff_im = bwareaopen(diff_im,300);
    
    % Label all the connected components in the image.
    bw = bwlabel(diff_im, 8);
    
    % We get a set of properties for each labeled region.
    stats = regionprops(bw, 'BoundingBox', 'Centroid');
    
    % Display the image
    imshow(data)
    
    hold on
    %This is a loop to bound the red objects in a rectangular box.
    for object = 1:length(stats)
        bb = stats(object).BoundingBox;
        bc = stats(object).Centroid;
        disp(double(bc));
       
        if bc(1)>420
            fwrite(s,'0');
        end
        if bc(1)<370
            fwrite(s,'1');
        end
        if 370<bc(1)<420
            fwrite(s,'2');
        end
        if bc(2)>385
            fwrite(s,'3');
        end
        if bc(2)<310
            fwrite(s,'4');
        end
        if 310<bc(2)<385
            fwrite(s,'5');
        end
        if 370<bc(1)<420&&310<bc(2)<385
            fwrite(s,'6');
        end
        rectangle('Position',bb,'EdgeColor','r','LineWidth',2)
        plot(bc(1),bc(2), '-m+')
        a=text(bc(1)+15,bc(2), strcat('X: ', num2str(round(bc(1))), '    Y: ', num2str(round(bc(2)))));
        set(a, 'FontName', 'Arial', 'FontWeight', 'bold', 'FontSize', 12, 'Color', 'yellow');
    end
    fwrite(s,'1');
    hold off
end

% Stop the video aquisition.
stop(vid);
fclose(s);
delete(s);
flushdata(vid);
clear all;
close all;