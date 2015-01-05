﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using UVEngine;
using Microsoft.Live;
using Microsoft.Phone.Controls;

namespace UVEngine
{
    public partial class BrowseSkyDrive : PhoneApplicationPage
    {
        private static readonly string[] DocFileTypes;
        private static readonly string[] AudioFileTypes;
        private static readonly string[] VideoFileTypes;
        private static string fileID="0";
        private LiveConnectClient liveClient;
        private static LiveConnectSession session;
        static BrowseSkyDrive()
        {
            DocFileTypes = new string[]
            {
                "pptx",
                "ppsx",
                "ppt",
                "pot",
                "pps",
                "pptm",
                "potm",
                "potx",
                "ppsm",
                "doc",
                "docx",
                "xlsx",
                "xls",
                "xlsm",
                "xlsb",
                "docm",
                "dot",
                "dotx",
                "one",
                "pdf"
            };

            AudioFileTypes = new string[] { "mp3" };
            VideoFileTypes = new string[] { "mp4", "wav", "mwv" };
        }
        public static string FileID
        {
            get
            {
                return BrowseSkyDrive.fileID;
            }
        }
        public static LiveConnectSession Session
        {
            get
            {
                return BrowseSkyDrive.session;
            }
        }

        public BrowseSkyDrive()
        {
            InitializeComponent();
            this.Loaded += this.OnLoaded;
        }

        private async void OnLoaded(object sender, RoutedEventArgs routedEventArgs)
        {
            this.Loaded -= this.OnLoaded;
            this.liveClient = new LiveConnectClient(Skydrive.Session);
            try
            {
                LiveOperationResult operationResult = await this.liveClient.GetAsync("/me/skydrive/files");
                dynamic result = operationResult.Result;
                if (result.data == null)
                {
                    this.ShowError("Server did not return a valid response.");
                    return;
                }

                dynamic items = result.data;
                this.ListBox.Items.Clear();
                foreach (dynamic item in items)
                {
                    var newItem = new SkyDriveItem(item);

                    var itemTile = new SkyDriveItemTile()
                    {
                        ItemName = newItem.Name,
                        DataContext = newItem,
                    };

                    itemTile.OnItemSelect += itemTile_OnItemSelect;

                    this.ListBox.Items.Add(itemTile);
                }
            }
            catch (LiveConnectException e)
            {
                this.ShowError(e.Message);
            }
        }

        private void itemTile_OnItemSelect(object sender, RoutedEventArgs e)
        {
            var selectedTile = sender as SkyDriveItemTile;
            if (selectedTile != null)
            {
                var sdItem = selectedTile.DataContext as SkyDriveItem;
                if (sdItem != null && sdItem.IsFolder)
                {
                    this.liveClient.GetAsync(sdItem.ID + "/files");
                }
                else
                {
                    //string url;
                    //if (IsVideoFile(sdItem.Name))
                    //{
                    //    url = string.Format("/PlayVideo.xaml?name={0}&id={1}", sdItem.Name, sdItem.ID);
                    //}
                    //else if (IsAudioFile(sdItem.Name))
                    //{
                    //    url = string.Format("/PlayAudio.xaml?name={0}&id={1}", sdItem.Name, sdItem.ID);
                    //}
                    //else if (IsOfficeDocument(sdItem.Name))
                    //{
                    //    url = string.Format("/ViewDocument.xaml?name={0}&id={1}", sdItem.Name, sdItem.ID);
                    //}
                    //else
                    //{
                    //    url = string.Format("/ViewFile.xaml?name={0}&id={1}", sdItem.Name, sdItem.ID);
                    //}

                    //this.NavigationService.Navigate(new Uri(url, UriKind.Relative));
                    fileID = sdItem.ID;
                    this.NavigationService.RemoveBackEntry();
                    //this.NavigationService.GoBack();
                    this.NavigationService.Navigate(new Uri("/System/Skydrive.xaml", UriKind.Relative));
                }
            }
        }

        private void ShowError(string message)
        {
            MessageBox.Show("An Error Occurred...\n" + message);
            //this.NavigationService.Navigate(new Uri("/Error.xaml?msg=" + message, UriKind.Relative));
        }

        private static bool IsVideoFile(string fileName)
        {
            string[] segments = fileName.Split(new char[] { '.' }, StringSplitOptions.RemoveEmptyEntries);
            string extention = segments[segments.Length - 1];

            return VideoFileTypes.Contains(extention.ToLowerInvariant());
        }

        private static bool IsAudioFile(string fileName)
        {
            string[] segments = fileName.Split(new char[] { '.' }, StringSplitOptions.RemoveEmptyEntries);
            string extention = segments[segments.Length - 1];

            return AudioFileTypes.Contains(extention.ToLowerInvariant());
        }

        private static bool IsOfficeDocument(string fileName)
        {
            string[] segments = fileName.Split(new char[] { '.' }, StringSplitOptions.RemoveEmptyEntries);
            string extention = segments[segments.Length - 1];

            return DocFileTypes.Contains(extention.ToLowerInvariant());
        }

        private class SkyDriveItem
        {
            public SkyDriveItem(IDictionary<string, object> properties)
            {
                if (properties.ContainsKey("id"))
                {
                    this.ID = properties["id"] as string;
                }

                if (properties.ContainsKey("name"))
                {
                    this.Name = properties["name"] as string;
                }

                if (properties.ContainsKey("type"))
                {
                    this.ItemType = properties["type"] as string;
                }
            }

            public string ID { get; private set; }

            public string Name { get; private set; }

            public string ItemType { get; private set; }

            public bool IsFolder
            {
                get
                {
                    return !string.IsNullOrEmpty(this.ItemType) &&
                           (this.ItemType.Equals("folder") || this.ItemType.Equals("album"));
                }
            }

            public override string ToString()
            {
                return this.Name;
            }
        }
    }
}