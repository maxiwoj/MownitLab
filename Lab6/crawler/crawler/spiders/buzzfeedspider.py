# -*- coding: utf-8 -*-
"""Spider for BuzzFeed."""
import scrapy


class BuzzfeedspiderSpider(scrapy.Spider):
    """."""

    name = 'buzzfeedspider'
    allowed_domains = ['buzzfeed.com']
    start_urls = ['https://www.buzzfeed.com/archive/2017/' + str(m) + '/' + str(d) for m in range(1, 13) for d in range(1, 31)]

    number_of_articles = 0

    def parse(self, response):
        """."""
        links = response.css('div .js-card__content a.link-gray::attr(href)').extract()
        for link in links:
            yield scrapy.Request('http://www.buzzfeed.com' + link, self.parse_item)

    def parse_item(self, response):
        """Parse single news."""
        article_info = {}
        article_info["id"] = str(self.number_of_articles)
        self.number_of_articles += 1
        article_info["url"] = response.xpath('/html/head/link[contains(@rel, "canonical")]/@href').extract_first(default='response.url')
        article_info["title"] = response.css('.buzz-title::text').extract_first().strip()
        article_info["first_paragraph"] = response.css('.buzz-dek::text').extract_first().strip()
        article_info["text"] = ' '.join(response.css('.article p::text, .article h3::text').extract()).strip() +\
                               ' '.join(response.css("article img::attr(alt)").extract()) +\
                               ' '.join(response.css("article span.js-subbuzz__title-text::text").extract()) +\
                               ' '.join(response.css("article i::text").extract())

        if response.css("html::attr(lang)").extract_first() == 'en':
            yield article_info
